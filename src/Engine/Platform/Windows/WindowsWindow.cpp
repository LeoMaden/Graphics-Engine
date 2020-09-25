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
			// Window events.
			case WM_CLOSE:			senderWindow->Close();					return 0;
			case WM_DESTROY:		PostQuitMessage(0);						return 0;
			case WM_SIZE:			senderWindow->OnResize(lParam);			return 0;
			// Key events.
			case WM_KEYDOWN:		senderWindow->OnKeyDown(wParam, lParam);	return 0;
			case WM_KEYUP:			senderWindow->OnKeyUp(wParam, lParam);		return 0;
				// Mouse events.
			case WM_MOUSEMOVE:		senderWindow->OnMouseMove(wParam, lParam);		return 0;
			case WM_LBUTTONDOWN:	senderWindow->OnMouse1Down(wParam, lParam);		return 0;
			case WM_MOUSEWHEEL:		senderWindow->OnMouseScroll(wParam, lParam);	return 0;
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
	}


	RenderContext* WindowsWindow::CreateRenderContext(const RenderContextProperties& props)
	{
		RenderingApi api = props.GetRenderingApi();
		switch (api)
		{
			case RenderingApi::OpenGL:
			{
				const OpenGLContextProperties & glProps = *(OpenGLContextProperties*)&props;
				return CreateOpenGLContext(glProps);
			}
			default:
			{
				ASSERT(false, "Rendering API not supported");
				return nullptr;
			}
		}
	}

	void WindowsWindow::DeleteRenderContext(RenderContext* context)
	{
		RenderingApi api = context->GetRenderingApi();
		switch (api)
		{
			case RenderingApi::OpenGL:
			{
				OpenGLContext* glContext = (OpenGLContext*)context;
				DeleteOpenGLContext(glContext);
				break;
			}
			default:
			{
				ASSERT(false, "Rendering API not supported");
				break;
			}
		}
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

	void WindowsWindow::OnResize(LPARAM lParam)
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		if (!wglGetCurrentContext())
		{
			return;
		}

		glViewport(0, 0, width, height);

		WindowResizeEvent e(width, height);
		m_Callback(e);
	}

	void WindowsWindow::OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		KeyCode code = GetEngineKeyCode(wParam);
		KeyFlags flags = GetKeyFlags(lParam);
		KeyDownEvent e(code, flags);
		m_Callback(e);
	}

	void WindowsWindow::OnKeyUp(WPARAM wParam, LPARAM lParam)
	{
		KeyCode code = GetEngineKeyCode(wParam);
		KeyUpEvent e(code);
		m_Callback(e);
	}

	void WindowsWindow::OnMouse1Down(WPARAM wParam, LPARAM lParam)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		ModifierKeys mods = GetModKeys(wParam);

		MouseDownEvent e(KeyCode::Mouse1, { xPos, yPos }, mods);
		m_Callback(e);
	}

	void WindowsWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		ModifierKeys mods = GetModKeys(wParam);

		MouseMoveEvent e({ xPos, yPos }, mods);
		m_Callback(e);
	}

	void WindowsWindow::OnMouseScroll(WPARAM wParam, LPARAM lParam)
	{
		int dist = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		ModifierKeys modKey = GetModKeys(wParam);
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		MouseScrollEvent e(dist, modKey, { xPos, yPos });
		m_Callback(e);
	}

	List<Pair<KeyCode, WPARAM>> WindowsWindow::m_KeyCodeConversion
	{
		{ KeyCode::A, 0x41 },
		{ KeyCode::B, 0x42 },
		{ KeyCode::C, 0x43 },
		{ KeyCode::D, 0x44 },
		{ KeyCode::E, 0x45 },
		{ KeyCode::F, 0x46 },
		{ KeyCode::G, 0x47 },
		{ KeyCode::H, 0x48 },
		{ KeyCode::I, 0x49 },
		{ KeyCode::J, 0x4A },
		{ KeyCode::K, 0x4B },
		{ KeyCode::L, 0x4C },
		{ KeyCode::M, 0x4D },
		{ KeyCode::N, 0x4E },
		{ KeyCode::O, 0x4F },
		{ KeyCode::P, 0x50 },
		{ KeyCode::Q, 0x51 },
		{ KeyCode::R, 0x52 },
		{ KeyCode::S, 0x53 },
		{ KeyCode::T, 0x54 },
		{ KeyCode::U, 0x55 },
		{ KeyCode::V, 0x56 },
		{ KeyCode::W, 0x57 },
		{ KeyCode::X, 0x58 },
		{ KeyCode::Y, 0x59 },
		{ KeyCode::Z, 0x5A },

		{ KeyCode::LShift,		VK_SHIFT },
		{ KeyCode::LCtrl,		VK_CONTROL },
		{ KeyCode::Enter,		VK_RETURN },
		{ KeyCode::Backspace,	VK_BACK },
		{ KeyCode::Escape,		VK_ESCAPE },
		{ KeyCode::Spacebar,	VK_SPACE },

		{ KeyCode::D0, 0x30 },
		{ KeyCode::D1, 0x31 },
		{ KeyCode::D2, 0x32 },
		{ KeyCode::D3, 0x33 },
		{ KeyCode::D4, 0x34 },
		{ KeyCode::D5, 0x35 },
		{ KeyCode::D6, 0x36 },
		{ KeyCode::D7, 0x37 },
		{ KeyCode::D8, 0x38 },
		{ KeyCode::D9, 0x39 },

		{ KeyCode::Mouse1, VK_LBUTTON },
		{ KeyCode::Mouse2, VK_RBUTTON },
		{ KeyCode::Mouse3, VK_MBUTTON },

		{ KeyCode::Up,		VK_UP },
		{ KeyCode::Down,	VK_DOWN },
		{ KeyCode::Left,	VK_LEFT },
		{ KeyCode::Right,	VK_RIGHT },

		{ KeyCode::KP0, 0x60 },
		{ KeyCode::KP1, 0x61 },
		{ KeyCode::KP2, 0x62 },
		{ KeyCode::KP3, 0x63 },
		{ KeyCode::KP4, 0x64 },
		{ KeyCode::KP5, 0x65 },
		{ KeyCode::KP6, 0x66 },
		{ KeyCode::KP7, 0x67 },
		{ KeyCode::KP8, 0x68 },
		{ KeyCode::KP9, 0x69 }
	};

	Map<KeyCode, WPARAM> WindowsWindow::m_WinKeyCodeMap = []()
	{
		std::unordered_map<KeyCode, WPARAM> codes;
		for (auto pair : m_KeyCodeConversion)
		{
			codes[pair.first] = pair.second;
		}
		return codes;
	}();

	Map<WPARAM, KeyCode> WindowsWindow::m_EngineKeyCodeMap = []()
	{
		std::unordered_map<WPARAM, KeyCode> codes;
		for (auto pair : m_KeyCodeConversion)
		{
			codes[pair.second] = pair.first;
		}
		return codes;
	}();

	WPARAM WindowsWindow::GetWinKeyCode(KeyCode code)
	{
		return m_WinKeyCodeMap[code];
	}

	KeyCode WindowsWindow::GetEngineKeyCode(WPARAM winCode)
	{
		return m_EngineKeyCodeMap[winCode];
	}

	KeyFlags WindowsWindow::GetKeyFlags(LPARAM lParam)
	{
		KeyFlags flags;
		flags.Extended = lParam & (1 << 24);

		return flags;
	}

	ModifierKeys WindowsWindow::GetModKeys(WPARAM wParam)
	{
		int fwKeys = GET_KEYSTATE_WPARAM(wParam);

		ModifierKeys m;
		m.Ctrl = fwKeys & MK_CONTROL;
		m.Shift = fwKeys & MK_SHIFT;
		m.Mouse1 = fwKeys & MK_LBUTTON;
		m.Mouse2 = fwKeys & MK_RBUTTON;
		m.Mouse3 = fwKeys & MK_MBUTTON;

		return m;
	}


	OpenGLContext* WindowsWindow::CreateOpenGLContext(const OpenGLContextProperties& props) const
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

		return new OpenGLContext(realRC, props);
	}

	void WindowsWindow::DeleteOpenGLContext(OpenGLContext* context) const
	{
		HGLRC rc = (HGLRC)context->GetHandle();

		if (wglGetCurrentContext() == rc)
		{
			wglMakeCurrent(m_DeviceContextHandle, NULL);
		}

		wglDeleteContext(rc);
	}

}