#include "Window.h"
#include "Log.h"

#include "Events/MouseEvents.h"

#include <windowsx.h>

#include <iostream>
#include <glad/glad.h>
#include <GL/wglext.h>
#include <GL/glext.h>

namespace Engine {

	static std::vector<std::pair<KeyCode, int>> KeyCodeConvertion		
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

	static std::unordered_map<KeyCode, int> GetWin32KeyCode = []()
	{
		std::unordered_map<KeyCode, int> codes;
		for (auto pair : KeyCodeConvertion)
		{
			codes[pair.first] = pair.second;
		}
		return codes;
	}();

	static std::unordered_map<int, KeyCode> GetEngineKeyCode = []()
	{
		std::unordered_map<int, KeyCode> codes;
		for (auto pair : KeyCodeConvertion)
		{
			codes[pair.second] = pair.first;
		}
		return codes;
	}();


	static KeyFlags GetKeyFlags(LPARAM lParam)
	{
		KeyFlags flags;
		flags.Extended = lParam & (1 << 24);

		return flags;
	}

	static void OnKeyDown(Window* sender, WPARAM wParam, LPARAM lParam)
	{
		KeyCode code = GetEngineKeyCode[wParam];
		KeyFlags flags = GetKeyFlags(lParam);
		KeyDownEvent e(code, flags);
		sender->OnEvent(e);
	}

	static void OnKeyUp(Window* sender, WPARAM wParam, LPARAM lParam)
	{
		KeyCode code = GetEngineKeyCode[wParam];
		KeyUpEvent e(code);
		sender->OnEvent(e);
	}

	static void OnMouseMove(Window* sender, WPARAM wParam, LPARAM lParam)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		MouseMoveEvent e(xPos, yPos);
		sender->OnEvent(e);
	}

	/*static*/ LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* senderWindow;
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			senderWindow = (Window*)(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)senderWindow);
		}
		else
		{
			senderWindow = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		switch (uMsg)
		{
		case WM_CLOSE:			senderWindow->OnClose();		return 0;
		case WM_DESTROY:		PostQuitMessage(0);				return 0;

		// Key events.
		case WM_KEYDOWN:		OnKeyDown(senderWindow, wParam, lParam);	return 0;
		case WM_KEYUP:			OnKeyUp(senderWindow, wParam, lParam);		return 0;

		// Mouse events.
		case WM_MOUSEMOVE:		OnMouseMove(senderWindow, wParam, lParam);	return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	static constexpr LONG DEFAULT_WINDOW_HEIGHT = 720;
	static constexpr LONG DEFAULT_WINDOW_WIDTH = 1280;

	bool Window::Create()
	{
		// Register the window class.
		WNDCLASS wc = { };

		wc.lpfnWndProc = WindowProc;
		wc.hInstance = NULL;
		wc.lpszClassName = L"Main Window";
		wc.style = CS_OWNDC;			// Unique context for each window in class

		RegisterClass(&wc);

		// Create the window.
		RECT winSize;
		winSize.top = 0;
		winSize.left = 0;
		winSize.bottom = DEFAULT_WINDOW_HEIGHT;
		winSize.right = DEFAULT_WINDOW_WIDTH;

		AdjustWindowRect(&winSize, WS_OVERLAPPEDWINDOW, FALSE);

		HWND hwnd = CreateWindowEx(
			0,								// Optional window styles.
			wc.lpszClassName,				// Window class
			L"Main Window",					// Window text
			WS_OVERLAPPEDWINDOW,			// Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, winSize.right, winSize.bottom,

			NULL,			// Parent window	
			NULL,			// Menu
			NULL,			// Instance handle
			(LPVOID)this	// Additional application data
		);

		if (hwnd == NULL)
		{
			return false;
		}

		m_HWnd = hwnd;

		ShowWindow(hwnd, SW_SHOW);

		return true;
	}

	bool Window::CreateContext()
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

		HDC realDC = GetDC(m_HWnd);
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

		const int major_min = 4, minor_min = 5;
		int  contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		HGLRC realRC = wglCreateContextAttribsARB(realDC, 0, contextAttribs);
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

	/*	std::cout << "LOADED OPEN GL VERSION: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;*/

		glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

		return true;
	}

	void Window::PollEvents()
	{
		MSG msg = { };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::SwapBuffers()
	{
		::SwapBuffers(GetDC(m_HWnd));
	}



	bool Window::IsKeyDown(KeyCode key)
	{
		WPARAM winCode = GetWin32KeyCode[key];
		int16_t state = GetKeyState(winCode);
		return state & (1 << 15);
	}

	/*protected*/ void Window::OnClose()
	{
		m_ShouldClose = true;
		DestroyWindow(m_HWnd);
	}

}