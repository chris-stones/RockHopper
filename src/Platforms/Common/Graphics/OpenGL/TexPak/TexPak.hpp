
#pragma once

#include <rh_texture_loader.h> /* Checkout: https://github.com/chris-stones/rh_texture_pack.git */

#include <type_traits>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <memory>

namespace RH { namespace Libs { namespace TexPak {

	class Texture {

		rh_texpak_handle handle;
		rh_texpak_idx    idx;

	public:

		Texture(rh_texpak_handle handle, rh_texpak_idx idx)
			:	handle(handle),
			 	idx(idx)
		{
		}

		~Texture() {

			rh_texpak_release( idx );
		}

		void ReadCoords(int dim, int stride, float * coords) const {

			rh_texpak_get_coords(idx, dim, stride, coords);
		}

		float GetWidth() const {

			unsigned int w,h;
			rh_texpak_get_size(idx,&w,&h);
			return static_cast<float>(w);
		}

		float GetHeight() const {

			unsigned int w,h;
			rh_texpak_get_size(idx,&w,&h);
			return static_cast<float>(h);
		}

		void GetSize(float &w, float &h) const {

			unsigned int iw,ih;
			rh_texpak_get_size(idx,&iw,&ih);
			w = static_cast<float>(iw);
			h = static_cast<float>(iw);
		}

		GLenum GetTarget() const {

			GLenum target;
			rh_texpak_get_textarget(handle, &target);
			return target;
		}

		GLuint GetTexture() const {

			GLuint t = 0;
			rh_texpak_get_texture(idx, &t);
			return t;
		}
	};

	class TexturePackage {

		rh_texpak_handle handle {NULL};
		bool isloaded {false};

	public:

		class OpenException 	: public std::exception {public: const char * what() const throw() { return "TexturePackage::OpenException"; } };
		class LoadException 	: public std::exception {public: const char * what() const throw() { return "TexturePackage::LoadException"; } };
		class LookupException 	: public std::runtime_error {public: LookupException(const std::string & s) : std::runtime_error(s) {} };

		typedef enum {

			FILESYSTEM 				= RH_TEXPAK_FILESYSTEM,				/* load from the file-system */
			ANDROID_APK 			= RH_TEXPAK_ANDROID_APK,			/* load from android APK */
			PLATFORM_DEFAULT		= RH_TEXPAK_APP,					/* load from default for this platform */
			ENABLE_TEXTURE_ARRAY 	= RH_TEXPAK_ENABLE_TEXTURE_ARRAY,	/* enable use of GL_EXT_texture_array */
		} texture_pak_flags_enum_t;

		TexturePackage( const char * pakfile, int flags) {

			if( rh_texpak_open( pakfile, &handle, flags ) != 0 )
				throw OpenException();

			Load(); // TODO:
		}

		~TexturePackage() {

			// TODO: proper logging!
			int e = rh_texpak_forceclose(handle);
			if( e != 0 )
				printf("WARNING: TexPak: is being closed with %d open handles\n", e);

			assert(e==0);
		}

		void Load() {

			// constructor opens the texture pack.
			//	but here we actually load the texture data.

			if(isloaded)
				return;

			if(rh_texpak_load(handle) != 0)
				throw LoadException();

			isloaded = true;
		}

		Texture * GetTexture(const char * name) {

			rh_texpak_idx idx;

			if( rh_texpak_get(handle, name, &idx) == 0) {

				try {
					return new Texture( handle, idx );

				} catch( const std::exception &) {

					rh_texpak_release(idx);
					throw;
				}
			}
			else
				throw LookupException(name);
		}

		GLenum GetTarget() const {

			GLenum target;
			rh_texpak_get_textarget(handle, &target);
			return target;
		}
	};

}}}

