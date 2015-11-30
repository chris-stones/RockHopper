
#pragma once

#if(HAVE_MOTIONVIDEO_PROVIDER)

	#if(HAVE_KNIB_VIDEO)
		#include "MotionVideoEngines/Knib.hpp"
	#endif

	#if(HAVE_THEORA_VIDEO)
		#include "MotionVideoEngines/Theora.hpp"
	#endif

#else

// NO MOTIONVIDEO PROVIDER SUPPORTED!

#endif
