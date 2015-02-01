
#pragma once

class Win32Input_ {

	static RH::Libs::EventDispatcher::EventPublicationManager eventPublicationManager;

	RH::UI::Events::keys ToKey() {

		// TODO:
		return RH::UI::Events::KEY_UNKNOWN;
	}

//	template<typename _T>
//	void PublishEvent(XKeyEvent * xkeyevent) {
//
//		eventPublicationManager.PublishEvent(_T(ToKey(xkeyevent)));
//	}

public:

	Win32Input_() {

	}

	void DispatchInputEvents() {

	}

};

