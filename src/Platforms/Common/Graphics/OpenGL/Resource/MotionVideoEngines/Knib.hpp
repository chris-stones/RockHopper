#pragma once

/*** https://github.com/chris-stones/KnibVideo ***/
#include <knib_read.h>

/*** https://github.com/chris-stones/libimg ***/
//#include <libimg.h>

#include <Graphics/Graphics.hpp>
#include <Libs/Concurrency/IConcurrentJob.hpp>
#include <Libs/Concurrency/IConcurrentJobQueue.hpp>
#include <stdexcept>
#include <mutex>

namespace RH { namespace Graphics { namespace Abstract {

class MotionVideo::Impl : public RH::Libs::Concurrency::IConcurrentJob  {

	knib_handle knib;
	std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> concurrentJobQueue;

	int channels;

	GLuint textures[4];

	std::mutex mutex;
	bool decode_in_progress {false};
	bool frame_is_ready {false};

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

		if(channels>=3)
			return CrCbAdjustResolution(((rawSize+7)/8)*8,channel);

		return ((rawSize+3)/4)*4;
	}

	GLenum GetInternalFormat() {

		const int kflags = knib_flags(knib);
		switch(kflags & KNIB_TEX_MASK)
		{
		default:
			return 0;
		case KNIB_TEX_DXT1:
			return COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case KNIB_TEX_ETC1:
			return ETC1_RGB8_OES;
		}
	}

	int CreateTextures() {

		int w,h;

		knib_get_dimensions(knib,&w,&h);

		w = TexSize(w,0);
		h = TexSize(h,0);

		memset(textures, 0, sizeof textures);
		GL_ERROR();
		glGenTextures(this->channels, textures);
		GL_ERROR();

		GLenum internalFormat = GetInternalFormat();

		for(int channel=0;channel<this->channels; channel++) {

			const int channel_w = CrCbAdjustResolution(w,channel);
			const int channel_h = CrCbAdjustResolution(h,channel);

			// FIXME: Assuming 8 bytes per block (ETC1 or DXT1).
			const int compressed_block_size = 8;

			// FIXME: Assuming block compression of 4x4.
			const int channel_size = compressed_block_size * ((channel_w+3)/4) * ((channel_h+3)/4);

			glActiveTexture(GL_TEXTURE0 + channel);
			glBindTexture(GL_TEXTURE_2D, textures[channel]);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			GL_ERROR();

			glCompressedTexImage2D(
			  GL_TEXTURE_2D,
			  0,
			  internalFormat,
			  channel_w,
			  channel_h,
			  0,
			  channel_size,
			  NULL);

			GL_ERROR();
		}

		return 0;
	}

	void UpdateTextures(void * datas[], int sizes[]) {

		int w,h;
		knib_get_dimensions(knib,&w,&h);

		w = TexSize(w,0);
		h = TexSize(h,0);

		GLenum internalFormat = GetInternalFormat();

		for(int channel=0;channel<this->channels; channel++) {

			if(sizes[channel]) {

				const int channel_w = CrCbAdjustResolution(w,channel);
				const int channel_h = CrCbAdjustResolution(h,channel);

				glActiveTexture(GL_TEXTURE0 + channel);
				glBindTexture(GL_TEXTURE_2D, textures[channel]);

				GL_ERROR();

#if(HAVE_OPENGL)
				glCompressedTexSubImage2D(
					  GL_TEXTURE_2D, // TARGET
					  0,  // LEVEL
					  0, 0, // XOFF, YOFF
					  channel_w,
					  channel_h,
					  internalFormat,
					  sizes[channel],
					  datas[channel]
					);
#elif HAVE_GLES2
				// glCompressedTexSubImage2D doesn't seem to work on the rapsberry pi !??
				glCompressedTexImage2D(
					  GL_TEXTURE_2D,
					  0,
					  internalFormat,
					  channel_w,
					  channel_h,
					  0,
					  sizes[channel],
					  datas[channel]);
#else
	#error no way to update texture!
#endif

				if( GL_ERROR() ) {

					printf("\tchan %d\n", channel);
					printf("\t%dx%d\n", channel_w, channel_h);
					printf("\t%p (%d)\n", datas[channel], sizes[channel]);
				}
			}
		}
	}

	void UpdateTextures() {

		void * data[4];
		int    size[4];

		knib_get_frame_data(knib,
			data+0, size+0,
			data+1, size+1,
			data+2, size+2,
			data+3, size+3);

		UpdateTextures(data,size);
	}

public:

	std::shared_ptr<Impl> sthis; // FIXME: ugly!

	Impl(const std::string & s, std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> concurrentJobQueue)
		:	concurrentJobQueue(concurrentJobQueue)
	{

		if( knib_open_file(s.c_str(), &knib) != 0)
			throw std::runtime_error("knib_open_file failed.");

		int flags = knib_flags(knib);

		if((flags & KNIB_CHANNELS_MASK)==KNIB_CHANNELS_PACKED) {
			if(flags & KNIB_ALPHA)
				channels = 2;
			else
				channels = 1;
		}
		else {
			if(flags & KNIB_ALPHA)
				channels = 4;
			else
				channels = 3;
		}

		CreateTextures();
		UpdateTextures();
	}

	~Impl() {

		glDeleteTextures(channels, textures);

		knib_close(knib);
	}

	virtual void ConcurrentJob() override {

		knib_next_frame(knib);

		{
			std::unique_lock<std::mutex> lock( mutex );
			decode_in_progress = false;
			frame_is_ready = true;
		}
	}

	void NextFrame() {

		std::unique_lock<std::mutex> lock( mutex );

		if(frame_is_ready) {

			if(channels >= 3) {
				if(!(knib_current_frame(knib)%3))
					UpdateTextures();
			}
			else
				UpdateTextures();

			frame_is_ready = false;
		}

		if(!decode_in_progress) {
			decode_in_progress = true;
			concurrentJobQueue->AddJob(sthis);
		}
	}

	GLuint GetRGBTexture() const {
		if(channels>=1)
			return textures[0];
		return 0;
	}

	GLuint GetYTexture() const {
		if(channels>=3)
			return textures[0];
		return 0;
	}

	GLuint GetCbTexture() const {
		if(channels>=3)
			return textures[1];
		return 0;
	}

	GLuint GetCrTexture() const {
		if(channels>=3)
			return textures[2];
		return 0;
	}

	GLuint GetATexture() const {
		if(HasAlphaChannel())
			return textures[channels-1];
		return 0;
	}

	int ShaderIndex() const {

		return knib_current_frame(knib) % 3;
	}

	float GetWidth() const {
		int w,h;
		knib_get_dimensions(knib, &w, &h);
		return static_cast<float>(w);
	}

	float GetHeight() const {
		int w,h;
		knib_get_dimensions(knib, &w, &h);
		return static_cast<float>(h);
	}

	void ReadCoords(int dim, int stride, float * coords) const {

		const int TOP_LEFT  = 0;
		const int BOT_LEFT  = 1;
		const int TOP_RIGHT = 2;
		const int BOT_RIGHT = 3;
		const int X=0;
		const int Y=1;

		int vw,vh,tw,th;
		knib_get_dimensions(knib,&vw,&vh);
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

		return this->channels == 4 || this->channels==2;
	}
};

MotionVideo::MotionVideo(UpdatedNode * parent, const std::string & s, std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> cjq)
	:	Updatable(parent)
{
	std::shared_ptr<Impl> sthis =
		std::shared_ptr<Impl>( impl = new Impl(s,cjq) );

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

