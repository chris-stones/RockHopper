
class WGLContext_ {

	HDC dc;
	HGLRC rc;

public:

	WGLContext_(HWND hWnd) 
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof pfd);
		pfd.nSize = sizeof pfd;
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 16;
		this->dc = GetDC(hWnd);
		int nPixelFormat = ChoosePixelFormat(dc, &pfd);
		SetPixelFormat(dc, nPixelFormat, &pfd);
		rc = wglCreateContext(dc);
	}

	void MakeContextCurrent() {

		wglMakeCurrent(dc, rc);
	}

	virtual ~WGLContext_() {

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(rc);
	}
};

