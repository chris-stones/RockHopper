
#pragma once

#include <rh_raw_loader.h>
#include <libavformat/avformat.h>

#include <type_traits>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <memory>

class RawPakMedia {

	rh_rawpak_handle handle;
	rh_rawpak_ctx ctx;

public:

	class ReadException 		: public std::exception {public: const char * what() const throw() { return "RawPak::ReadException";     } };
	class SeekException 		: public std::exception {public: const char * what() const throw() { return "RawPak::SeekException";     } };
	class AvFormatException 	: public std::exception {public: const char * what() const throw() { return "RawPak::AvFormatException"; } };

	RawPakMedia( rh_rawpak_handle handle, rh_rawpak_ctx ctx )
		:	handle(handle),
			ctx(ctx)
	{}

	~RawPakMedia() {
		rh_rawpak_close_ctx( ctx );
	}

	int GetAvFormatContext(AVFormatContext & avformatcontext, size_t internalBufferSize=0) {

		if( rh_rawpak_open_avformatctx(ctx, internalBufferSize, (void**)&avformatcontext) != 0 )
			throw AvFormatException();

		return 0;
	}
};

class RawPakMediaPackage {

	rh_rawpak_handle handle;

	bool isloaded{false};

public:

	class OpenException 	: public std::exception {public: const char * what() const throw() { return "RawPak::OpenException"; } };
	class LoadException 	: public std::exception {public: const char * what() const throw() { return "RawPak::LoadException"; } };
	class LookupException 	: public std::runtime_error { using std::runtime_error::runtime_error; };

	typedef enum {

		FILESYSTEM 				= RH_RAWPAK_FILESYSTEM,				/* load from the file system */
		ANDROID_APK 			= RH_RAWPAK_ANDROID_APK,			/* load from android APK */
		PLATFORM_DEFAULT		= RH_RAWPAK_APP,					/* load from default for this platform */
	} texture_pak_flags_enum_t;

	RawPakMediaPackage(const char * pakfile, int flags) {

		if( rh_rawpak_open( pakfile, &handle, flags ) != 0 )
			throw OpenException();
	}

	RawPakMedia * GetMedia(const char * resourceName) {

		rh_rawpak_ctx _ctx = NULL;

		if( rh_rawpak_open_ctx(handle,resourceName, &_ctx) != 0)
			throw LookupException( resourceName );

		return new RawPakMedia(handle, _ctx);
	}

	virtual ~RawPakMediaPackage() {

		int e = rh_rawpak_close(handle);
		assert(e == 0);
	}
};

