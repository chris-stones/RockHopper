
class Win32Window_ {

	HWND hWnd;
	ATOM windowClass;

	DWORD ExStyle() const {
		return WS_EX_APPWINDOW;
	}

	DWORD WindowStyle() const {
		return WS_OVERLAPPEDWINDOW |
			WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS;
	}

	HINSTANCE Instance() const {
		return (HINSTANCE)(GetModuleHandle(NULL));
	}

	UINT ClassStyle() const {
		return CS_OWNDC;
	}

	ATOM ClassName() const {
		WNDCLASSEX wndClass;
		char classname[64];
		memset(&wndClass, 0, sizeof wndClass);
		sprintf_s(classname, "ClassName %p", this);
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = ClassStyle();
		wndClass.lpfnWndProc = &DefWindowProc;
		wndClass.hInstance = Instance();
		wndClass.hCursor = LoadCursor(NULL/*Instance()*/, IDC_ARROW);
		wndClass.lpszClassName = classname;
		return RegisterClassEx(&wndClass);
	}

public:
	Win32Window_(const RH::UI::WindowHints & windowHints) {

		this->windowClass = ClassName();

		this->hWnd = CreateWindowEx(
			ExStyle(),
			(LPCTSTR)((void*)(size_t)(this->windowClass)),
			TEXT("WIN32"),
			WindowStyle(),
			windowHints.GetX(), 
			windowHints.GetY(), 
			windowHints.GetW(), 
			windowHints.GetH(),
			NULL,
			NULL,
			Instance(),
			NULL);

		ShowWindow(this->hWnd, SW_SHOW);
	}

	HWND GetHWND() const { return hWnd; }
};

