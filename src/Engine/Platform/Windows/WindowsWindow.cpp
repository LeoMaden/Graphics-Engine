#include "Pch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>
#include <GL/wglext.h>
#include <GL/glext.h>

namespace Engine {

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* senderWindow;
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			senderWindow = (WindowsWindow*)(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)senderWindow);
		}
		else
		{
			senderWindow = (WindowsWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		switch (uMsg)
		{
			case WM_CLOSE:			senderWindow->Close();			return 0;
			case WM_DESTROY:		PostQuitMessage(0);				return 0;
			//// Key events.
			//case WM_KEYDOWN:		OnKeyDown(senderWindow, wParam, lParam);	return 0;
			//case WM_KEYUP:			OnKeyUp(senderWindow, wParam, lParam);		return 0;
			//// Mouse events.
			//case WM_MOUSEMOVE:		OnMouseMove(senderWindow, wParam, lParam);		return 0;
			//case WM_LBUTTONDOWN:	OnMouse1Down(senderWindow, wParam, lParam);		return 0;
			//case WM_MOUSEWHEEL:		OnMouseScroll(senderWindow, wParam, lParam);	return 0;
			//// Window events.
			//case WM_SIZE:			OnResize(senderWindow, lParam);			return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	WindowsWindow::WindowsWindow(Window::Properties props)
	{
		// Register the window class.
		WNDCLASS wc = { };
		wc.style = CS_OWNDC;				// Unique context for each window in class
		wc.lpfnWndProc = WindowProc;		// Window procedure.
		wc.hInstance = NULL;
		wc.lpszClassName = L"Window";

		RegisterClass(&wc);

		// Create the window.
		RECT winSize;
		winSize.left = 0;
		winSize.top = 0;
		winSize.right = props.Size.x;
		winSize.bottom = props.Size.y;

		AdjustWindowRect(&winSize, WS_OVERLAPPEDWINDOW, FALSE);
		LONG winWidth = winSize.right - winSize.left;
		LONG winHeight = winSize.bottom - winSize.top;

		HWND hwnd = CreateWindowEx(
			0,								// Optional window styles.
			wc.lpszClassName,				// Window class
			L"Main Window",					// Window text
			WS_OVERLAPPEDWINDOW,			// Window style

			// Size and position. 
			// TODO: Use props position.
			CW_USEDEFAULT, CW_USEDEFAULT, winWidth, winHeight,

			NULL,			// Parent window	
			NULL,			// Menu
			NULL,			// Instance handle
			(LPVOID)this	// Additional application data
		);

		ASSERT(hwnd, "Window was not created");
		m_WindowHandle = hwnd;
		m_DeviceContextHandle = GetDC(hwnd);

		ShowWindow(hwnd, SW_SHOW);
	}

	WindowsWindow::~WindowsWindow()
	{
		Close();
	}

	void WindowsWindow::CreateOpenGLContext(OpenGLContextProperties props)
	{
		// Fake WindowProc.
		auto fakeWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg)
			{
			case WM_CLOSE:			DestroyWindow(hwnd);		return (LRESULT)0;
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		};

		// Create fake window class.
		WNDCLASS fakeWC = { };

		fakeWC.lpfnWndProc = fakeWndProc;
		fakeWC.hInstance = NULL;
		fakeWC.lpszClassName = L"Fake Window";
		fakeWC.style = CS_OWNDC;				// Unique context for each window in class

		RegisterClass(&fakeWC);

		// Create fake window.
		HWND fakeWnd = CreateWindowEx(
			0,					  // Optional window styles.
			fakeWC.lpszClassName,	// Window class
			L"Fake Window",		 // Window text
			WS_OVERLAPPEDWINDOW,	// Window style
			0, 0, 1, 1,	 // Size and position
			NULL, NULL,	 // Parent window, Menu
			NULL, NULL	  // Instance handle, Additional application data
		);

		// Get fake device context.
		HDC fakeDC = GetDC(fakeWnd);

		PIXELFORMATDESCRIPTOR fakePfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	//Flags
			PFD_TYPE_RGBA,		// The kind of framebuffer. RGBA or palette.
			32,					// Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			24,					// Number of bits for the depthbuffer
			8,					// Number of bits for the stencilbuffer
			0,					// Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};

		int fakeFormat = ChoosePixelFormat(fakeDC, &fakePfd);
		ASSERT(fakeFormat, "Failed to choose pixel format");

		BOOL ok = SetPixelFormat(fakeDC, fakeFormat, &fakePfd);
		ASSERT(ok, "Failed to set pixel format");

		// Create fake OpenGL context.
		HGLRC fakeRC = wglCreateContext(fakeDC);
		ASSERT(fakeRC, "Failed to initialise OpenGL");

		ok = wglMakeCurrent(fakeDC, fakeRC);
		ASSERT(ok, "Failed to initialise OpenGL");

		gladLoadGL();

		// Load create real OpenGL context.
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		ASSERT(wglChoosePixelFormatARB, "Failed to get ARB Pixel Format extension");

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		ASSERT(wglCreateContextAttribsARB, "Failed to get ARB Context Attribs extension");

		HDC realDC = m_DeviceContextHandle;
		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		int pixelFormatID; UINT numFormats;
		ok = wglChoosePixelFormatARB(realDC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
		ASSERT(ok && numFormats, "Failed to choose ARB pixel format");

		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(realDC, pixelFormatID, sizeof(pfd), &pfd);
		SetPixelFormat(realDC, pixelFormatID, &pfd);

		List<int> contextAttribs = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, props.VersionMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB, props.VersionMinor
		};

		if (props.CoreProfile == true)
		{
			contextAttribs.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
			contextAttribs.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
		}

		if (props.DebugContext == true)
		{
			contextAttribs.push_back(WGL_CONTEXT_FLAGS_ARB);
			contextAttribs.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
		}

		contextAttribs.push_back(0);

		HGLRC realRC = wglCreateContextAttribsARB(realDC, 0, contextAttribs.data());
		ASSERT(realRC, "Failed to create ARB context attribs");

		// Delete fake context and window.
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakeWnd, fakeDC);
		DestroyWindow(fakeWnd);

		ASSERT(wglMakeCurrent(realDC, realRC), "Could not make OpenGL context current");

		gladLoadGL();

		LOG_INFO("Loaded Open GL version {}", glGetString(GL_VERSION));
		LOG_INFO("\tVendor {}", glGetString(GL_VENDOR));
		LOG_INFO("\tRenderer {}", glGetString(GL_RENDERER));

		glViewport(0, 0, props.ViewportSize.x, props.ViewportSize.y);
	}

	void WindowsWindow::SwapBuffers() const
	{
		::SwapBuffers(m_DeviceContextHandle);
	}

	void WindowsWindow::PollEvents() const
	{
		MSG msg = { };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::Show() const
	{
		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	void WindowsWindow::Hide() const
	{
		ShowWindow(m_WindowHandle, SW_HIDE);
	}

	Vector2 WindowsWindow::GetClientSize() const
	{
		RECT clientRect;
		GetClientRect(m_WindowHandle, &clientRect);

		return Vector2(clientRect.right, clientRect.bottom);
	}

	void WindowsWindow::Close()
	{
		DestroyWindow(m_WindowHandle);
		m_IsOpen = false;
	}

}