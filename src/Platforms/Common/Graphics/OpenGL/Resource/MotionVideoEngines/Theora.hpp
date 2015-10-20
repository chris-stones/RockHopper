#pragma once

#include <Graphics/Graphics.hpp>
#include <Libs/Concurrency/IConcurrentJob.hpp>
#include <Libs/Concurrency/IConcurrentJobQueue.hpp>
#include <stdexcept>
#include <map>

#include <ogg/ogg.h>
#include <theora/theoradec.h>

#include <mutex>

namespace RH { namespace Graphics { namespace Abstract {

class MotionVideo::Impl : public RH::Libs::Concurrency::IConcurrentJob {

	std::string file_name;
	FILE * file {nullptr};
	ogg_sync_state ogg_state;
	int ogg_stream_serial {0};
	GLuint textures[3];
	int w{0};
	int h{0};
	th_ycbcr_buffer codec_buffer;
	std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> concurrentJobQueue;

	std::mutex mutex;
	bool decode_in_progress {false};
	bool frame_is_ready {false};

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
			th_comment_clear(&mComment);
			th_info_clear(&mInfo);
			th_decode_free(mCtx);
		}
	};

	struct OggStream {
	  ogg_stream_state mState;
	  TheoraDecode mTheora;
	  bool initilaised{false};

	  OggStream() {}

	  void Init(int serial) {
		  int e = ogg_stream_init(&mState, serial);
		  assert(e==0);
		  initilaised=true;
	  }

	  ~OggStream() {
		  if(initilaised)
			  ogg_stream_clear(&mState);
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

		  int size = 256;

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

			if(this->oggStreamMap.find(serial)==this->oggStreamMap.end())
				this->oggStreamMap[serial].Init(serial);

			OggStream & oggStream = this->oggStreamMap[serial];

			int ret = ogg_stream_pagein(&oggStream.mState, page);
			assert(ret==0);

			ret = ogg_stream_packetout(&oggStream.mState, packet);
			if (ret == 0) {
				// Need more data to be able to complete the packet
				continue;
			}
			else if (ret == -1) {
				// We are out of sync and there is a gap in the data.
				// We lost a page somewhere.
				printf("ogg_stream_packetout err.\n");
				return false;
			}
			return true;
		}
		return false;
	}

public:

	std::shared_ptr<Impl> sthis; // FIXME: ugly!

	Impl(const std::string & file_name, std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> concurrentJobQueue)
		:	file_name(file_name),
			concurrentJobQueue(concurrentJobQueue)
	{

		if((this->file = fopen(file_name.c_str(), "rb"))==nullptr)
			throw std::runtime_error("fopen error.");

		memset(textures, 0, sizeof textures);
		memset(codec_buffer, 0, sizeof codec_buffer);

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

				  oggStream.mTheora.mCtx =
					th_decode_alloc(
						&oggStream.mTheora.mInfo,
						oggStream.mTheora.mSetup);

				  th_setup_free(oggStream.mTheora.mSetup);

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

					ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, this->codec_buffer);

					assert(ret==0);

					if(ret == 0) {
						CreateTextures(this->codec_buffer);
						UpdateTextures(this->codec_buffer);
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
		ogg_sync_clear(&ogg_state);

		glDeleteTextures(3, textures);
	}

	virtual void ConcurrentJob() override {

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

				ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, this->codec_buffer);

				if(ret == 0) {
					std::unique_lock<std::mutex> lock( mutex );
					this->frame_is_ready = true;
					this->decode_in_progress = false;
					return;
				}
				assert(false);
			}
		}

		while(this->read_ogg_packet(&packet, &page)) {

			int serial = this->ogg_stream_serial;

			OggStream & oggStream =
				this->oggStreamMap[serial];

			if(oggStream.mTheora.mCtx)
			{
				// Theora data-packet.
				int ret = 0;

				assert(ret==0);

				ogg_int64_t granulepos = -1;
				ret = th_decode_packetin(oggStream.mTheora.mCtx,
											 &packet,
											 &granulepos);

				ret = th_decode_ycbcr_out(oggStream.mTheora.mCtx, this->codec_buffer);

				if(ret == 0) {
					std::unique_lock<std::mutex> lock( mutex );
					this->decode_in_progress = false;
					this->frame_is_ready = true;
					return;
				}
				assert(false);
			}
		}

		{
			std::unique_lock<std::mutex> lock( mutex );
			this->decode_in_progress = false;
			return;
		}
	}

	void NextFrame() {

		std::unique_lock<std::mutex> lock( mutex );

		if(frame_is_ready) {
			UpdateTextures(this->codec_buffer);
			frame_is_ready = false;
		}

		if(!decode_in_progress) {

			decode_in_progress = true;
			concurrentJobQueue->AddJob(sthis);
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

MotionVideo::MotionVideo(UpdatedNode * parent, const std::string & s, std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> cjq)
	:	Updatable(parent)
{
	std::shared_ptr<Impl> sthis =
		std::shared_ptr<Impl>(impl = new Impl(s, cjq));

	sthis->sthis = sthis; // FIXME: Ugly!
}

MotionVideo::~MotionVideo() {

	impl->sthis.reset(); // FIXME: Ugly!
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

