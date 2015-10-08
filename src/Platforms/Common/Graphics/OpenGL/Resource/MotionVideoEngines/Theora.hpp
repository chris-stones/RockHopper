#pragma once

#include <Graphics/Graphics.hpp>
#include <stdexcept>
#include <map>

#include <ogg/ogg.h>
#include <theora/theoradec.h>

namespace RH { namespace Graphics { namespace Abstract {

class MotionVideo::Impl {

	FILE * file {nullptr};
	ogg_sync_state ogg_state;
	int ogg_stream_serial {0};
	GLuint textures[3];
	int w{0};
	int h{0};

	struct TheoraDecode {
		th_info mInfo;
		th_comment mComment;
		th_setup_info *mSetup {nullptr};
		th_dec_ctx* mCtx {nullptr};

		TheoraDecode() {
			th_info_init(&mInfo);
			th_comment_init(&mComment);
		}
		~TheoraDecode() {
			th_setup_free(mSetup);
			th_decode_free(mCtx);
		}
	};

	struct OggStream {
	  ogg_stream_state mState;
	  TheoraDecode mTheora;

	  OggStream() {
		  memset(&mState,  0, sizeof mState);
		  memset(&mTheora, 0, sizeof mTheora);
	  }
	};
	std::map<int, OggStream> oggStreamMap;

	static int CrCbAdjustResolution(int res,int channel) {

	  switch(channel) {
	  case 1:
	  case 2:
		  return (res+1)>>1;
	  default:
		  return res;
	  }
	}

	int TexSize(int rawSize, int channel) const {

		return CrCbAdjustResolution(((rawSize+1)/2)*2,channel);
	}

	int CreateTextures(th_ycbcr_buffer buffer) {

		GL_ERROR();
		glGenTextures(3, textures);
		GL_ERROR();

		for(int channel=0;channel<3; channel++) {

			const int channel_w = buffer[channel].stride;
			const int channel_h = buffer[channel].height;

			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, textures[channel]);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			GL_ERROR();

			glTexImage2D(
			  GL_TEXTURE_2D,
			  0,// LEVEL,
			  GL_LUMINANCE, // internal format
			  channel_w,
			  channel_h,
			  0, // border
			  GL_LUMINANCE, // format
			  GL_UNSIGNED_BYTE, // type
			  NULL);

			if(GL_ERROR()) {

				printf("ERR creating texture %dx%d\n",channel_w, channel_h);
			}
			else {
				printf("Created texture %d: %dx%d\n", channel,channel_w, channel_h);
			}
		}

		return 0;
	}

	void UpdateTextures(th_ycbcr_buffer buffer) {

		for(int channel=0;channel<3; channel++) {

			const int channel_w = buffer[channel].stride;
			const int channel_h = buffer[channel].height;

			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, textures[channel]);

			GL_ERROR();

			glTexSubImage2D(
			  GL_TEXTURE_2D,
			  0,0,0,
			  channel_w,
			  channel_h,
			  GL_LUMINANCE,
			  GL_UNSIGNED_BYTE,
			  (const void*)buffer[channel].data);

			if( GL_ERROR() ) {

				printf("\tchan %d\n", channel);
				printf("\t%dx%d\n", channel_w, channel_h);
				printf("\t%p\n", buffer[channel].data);
			}
		}
	}

	bool read_ogg_page(ogg_sync_state * state, ogg_page * p_ogg_page) {

		size_t bytes_total = 0;

		while(ogg_sync_pageout(state, p_ogg_page) != 1) {

		  int size = 4096;

//		  size = 64;// HACK

		  char* buffer = ogg_sync_buffer(state, 4096);

		  size_t bytes = fread(buffer, 1, 4096, this->file);
		  bytes_total += bytes;

		  if (bytes == 0) {


		    return false; // END OF FILE.
		  }

		  if(0 != ogg_sync_wrote(state, bytes)) {
			  printf("ogg_sync_wrote ERR\n");
			  return false; // ERROR
		  }
		}

		printf("read %d bytes, got a page\n", bytes_total);

		return true;
	}

	bool read_ogg_packet(ogg_packet *packet, ogg_page *page) {

		while(this->read_ogg_page(&ogg_state, page)) {

			int serial = ogg_page_serialno(page);

			OggStream & oggStream = this->oggStreamMap[serial];

			if (ogg_page_bos(page)) {
				printf("read_ogg_packet BOS\n");
				int e = ogg_stream_init(&oggStream.mState, serial);
				assert(e==0);
			}

			int ret = ogg_stream_pagein(&oggStream.mState, page);
			assert(ret==0);

			ret = ogg_stream_packetout(&oggStream.mState, packet);
			if (ret == 0) {
				// Need more data to be able to complete the packet
				printf("read_ogg_packet Need more data.\n");
				continue;
			}
			else if (ret == -1) {
				// We are out of sync and there is a gap in the data.
				// We lost a page somewhere.
				printf("ogg_stream_packetout err.\n");
				return false;
			}

			printf("Got a packet\n");
			return true;
		}
		return false;
	}

public:

	Impl(const std::string & s) {

		if((this->file = fopen(s.c_str(), "rb"))==nullptr)
			throw std::runtime_error("fopen error.");

		memset(textures, 0, sizeof textures);

		int e = ogg_sync_init(&ogg_state);
		assert(e==0);


		for(;;)
		{
			ogg_packet packet;
			ogg_page   page;

			if(read_ogg_packet(&packet, &page)) {

				int serial = ogg_page_serialno(&page);

				OggStream & oggStream =
					this->oggStreamMap[serial];

				int ret = th_decode_headerin(
					&oggStream.mTheora.mInfo,
					&oggStream.mTheora.mComment,
					&oggStream.mTheora.mSetup,
					&packet);

				if(ret < 0) {
					fclose(this->file);
					throw std::runtime_error("Theora error.");
				}
				else if (ret==0) {

				  printf("Allocate decoder\n");

				  oggStream.mTheora.mCtx =
					th_decode_alloc(
						&oggStream.mTheora.mInfo,
						oggStream.mTheora.mSetup);

				  if(!this->ogg_stream_serial)
					  this->ogg_stream_serial = serial;

				  assert(oggStream.mTheora.mCtx != nullptr);

				  this->w = oggStream.mTheora.mInfo.frame_width;
				  this->h = oggStream.mTheora.mInfo.frame_height;

				  {
					ogg_int64_t granulepos = -1;
					int ret = th_decode_packetin(oggStream.mTheora.mCtx,
												 &packet,
												 &granulepos);

					assert(ret==0);

					th_ycbcr_buffer buffer;
					ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, buffer);

					assert(ret==0);

					if(ret == 0) {
						CreateTextures(buffer);
						UpdateTextures(buffer);
					}
				  }

				  break;
				}
				else if (ret > 0) {

					// need more headers!
				}
			}
		}
	}

	~Impl() {

		fclose(this->file);

		glDeleteTextures(3, textures);
	}

	void NextFrame() {

		ogg_packet packet;
		ogg_page   page;

		{
			OggStream & oggStream =
				this->oggStreamMap[this->ogg_stream_serial];

			int ret;

			ret = ogg_stream_packetout(&oggStream.mState, &packet);

			if(ret==1) {
				ogg_int64_t granulepos = -1;
				ret = th_decode_packetin(oggStream.mTheora.mCtx,
											 &packet,
											 &granulepos);
				th_ycbcr_buffer buffer;
				ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, buffer);

				if(ret == 0) {
					UpdateTextures(buffer);
					printf("Got a frame.\n");
				}
				return;
			}
		}

		while(this->read_ogg_packet(&packet, &page)) {
//		if(read_ogg_packet(&packet, &page)) {

			int serial = this->ogg_stream_serial;

			OggStream & oggStream =
				this->oggStreamMap[serial];

			// if(correct stream !?
			if(oggStream.mTheora.mCtx)
			{
				// Theora data-packet.
				int ret = 0;

//				ret = th_decode_headerin(
//					&oggStream.mTheora.mInfo,
//					&oggStream.mTheora.mComment,
//					&oggStream.mTheora.mSetup,
//					&packet);
//
				assert(ret==0);

				ogg_int64_t granulepos = -1;
				ret = th_decode_packetin(oggStream.mTheora.mCtx,
				                             &packet,
				                             &granulepos);

//				assert(ret==0);

				th_ycbcr_buffer buffer;
				ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, buffer);

//				assert(ret==0);

				if(ret == 0) {
					UpdateTextures(buffer);
					printf("Got a frame.\n");
				}

				break;
			}
			else
			{
				printf("wrong stream?\n");
			}
		}
	}

	GLuint GetRGBTexture() const {
		return 0;
	}

	GLuint GetYTexture() const {
		return textures[0];
	}

	GLuint GetCbTexture() const {
		return textures[1];
	}

	GLuint GetCrTexture() const {
		return textures[2];
	}

	GLuint GetATexture() const {
		if(HasAlphaChannel())
			return GetYTexture(); // ALPHA HACK - Alpha shares Y texture!
		return 0;
	}

	int ShaderIndex() const {

		if(HasAlphaChannel())
			return 1;
		return 0;
	}

	float GetWidth() const {
		return static_cast<float>(this->w);
	}

	float GetHeight() const {
		return static_cast<float>(this->h);
	}

	void ReadCoords(int dim, int stride, float * coords) const {

		const int TOP_LEFT  = 0;
		const int BOT_LEFT  = 1;
		const int TOP_RIGHT = 2;
		const int BOT_RIGHT = 3;
		const int X=0;
		const int Y=1;

		int vw,vh,tw,th;
		vw = this->w;
		vh = this->h;
		tw = TexSize(vw,0);
		th = TexSize(vh,0);

		float w = static_cast<float>(vw) / static_cast<float>(tw);
		float h = static_cast<float>(vh) / static_cast<float>(th);

		coords[stride * TOP_LEFT  + X] = 0.0f;
		coords[stride * TOP_LEFT  + Y] = 0.0f;
		coords[stride * BOT_LEFT  + X] = 0.0f;
		coords[stride * BOT_LEFT  + Y] = h;
		coords[stride * TOP_RIGHT + X] = w;
		coords[stride * TOP_RIGHT + Y] = 0.0f;
		coords[stride * BOT_RIGHT + X] = w;
		coords[stride * BOT_RIGHT + Y] = h;
	}

	bool HasAlphaChannel() const {

		return false; // TODO
	}
};

MotionVideo::MotionVideo(UpdatedNode * parent, const std::string & s)
	:	Updatable(parent)
{
	impl = new Impl(s);
}

MotionVideo::~MotionVideo() {

	delete impl;
}

// IResource
float MotionVideo::GetWidth() const {

	return impl->GetWidth();
}

// IResource
float MotionVideo::GetHeight() const {

	return impl->GetHeight();
}

// IResource
void MotionVideo::ReadCoords(int dim, int stride, float * coords) const {

	impl->ReadCoords(dim, stride, coords);
}

void MotionVideo::NextFrame() {

	impl->NextFrame();
}

}}}

