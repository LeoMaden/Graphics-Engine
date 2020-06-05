#include "Window.h"

#include <iostream>
#include <glad/glad.h>
#include <GL/wglext.h>
#include <GL/glext.h>

namespace Engine {

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
        case WM_CLOSE:          senderWindow->OnClose();        return 0;
        case WM_DESTROY:        PostQuitMessage(0);             return 0;
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    ///*static*/ LRESULT Window::FakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    //{
    //    switch (uMsg)
    //    {
    //    }
    //    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    //}

    bool Window::Create()
	{
        // Register the window class.
        WNDCLASS wc = { };

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = NULL;
        wc.lpszClassName = L"Main Window";
        wc.style = CS_OWNDC;                // Unique context for each window in class

        RegisterClass(&wc);

        // Create the window.
        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            wc.lpszClassName,       // Window class
            L"Main Window",                 // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

            NULL,           // Parent window    
            NULL,           // Menu
            NULL,           // Instance handle
            (LPVOID)this    // Additional application data
        );

        if (hwnd == NULL)
        {
            return false;
        }

        m_HWnd = hwnd;

        ShowWindow(hwnd, SW_SHOW);

        return true;
	}

    void Window::RunSystemEventLoop()
    {
        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool Window::CreateContext()
    {
        // Fake WindowProc.
        auto fakeWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            switch (uMsg)
            {
            case WM_CLOSE:          DestroyWindow(hwnd);        return (LRESULT)0;
            }
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        };

        // Create fake window class.
        WNDCLASS fakeWC = { };

        fakeWC.lpfnWndProc = fakeWndProc;
        fakeWC.hInstance = NULL;
        fakeWC.lpszClassName = L"Fake Window";
        fakeWC.style = CS_OWNDC;                // Unique context for each window in class

        RegisterClass(&fakeWC);

        // Create fake window.
        HWND fakeWnd = CreateWindowEx(
            0,                      // Optional window styles.
            fakeWC.lpszClassName,   // Window class
            L"Fake Window",         // Window text
            WS_OVERLAPPEDWINDOW,    // Window style
            0, 0, 1, 1,     // Size and position
            NULL, NULL,     // Parent window, Menu
            NULL, NULL      // Instance handle, Additional application data
        );

        // Get fake device context.
        HDC fakeDC = GetDC(fakeWnd); 
        
        PIXELFORMATDESCRIPTOR fakePfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR), 1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE, 0, 0, 0, 0
        };

        int fakeFormat = ChoosePixelFormat(fakeDC, &fakePfd);
        if (!fakeFormat)
        {
            std::cerr << "ChoosePixelFormat() failed" << std::endl;
            return false;
        }
        if (!SetPixelFormat(fakeDC, fakeFormat, &fakePfd)) {
            std::cerr << "SetPixelFormat() failed" << std::endl;
            return false;
        }

        // Create fake OpenGL context.
        HGLRC fakeRC = wglCreateContext(fakeDC);
        if (!fakeRC) 
        {
            std::cerr << "wglCreateContext() failed" << std::endl;
            return false;
        }

        if (!wglMakeCurrent(fakeDC, fakeRC)) 
        {
            std::cerr << "wglMakeCurrent() failed" << std::endl;
            return false;
        }

        gladLoadGL();

        // Load create real OpenGL context.
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
        wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (!wglChoosePixelFormatARB) {
            std::cerr << "wglGetProcAddress() failed" << std::endl;
            return false;
        }
        
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
        wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (!wglCreateContextAttribsARB) {
            std::cerr << "wglGetProcAddress() failed" << std::endl;
            return false;
        }

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
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, 4,
            0
        };

        int pixelFormatID; UINT numFormats;
        bool status = wglChoosePixelFormatARB(realDC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

        if (!status || !numFormats) {
            std::cout << "wglChoosePixelFormatARB() failed." << std::endl;
            return false;
        }
        
        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(realDC, pixelFormatID, sizeof(pfd), &pfd);
        SetPixelFormat(realDC, pixelFormatID, &pfd);

        const int major_min = 3, minor_min = 0;
        int  contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
            WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        HGLRC realRC = wglCreateContextAttribsARB(realDC, 0, contextAttribs);
        if (!realRC) {
            std::cout << "wglCreateContextAttribsARB() failed." << std::endl;
            return false;
        }

        // Delete fake context and window.
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(fakeRC);
        ReleaseDC(fakeWnd, fakeDC);
        DestroyWindow(fakeWnd);

        if (!wglMakeCurrent(realDC, realRC))
        {
            std::cerr << "wglMakeCurrent() failed" << std::endl;
            return false;
        }

        gladLoadGL();

        char* version = (char*)glGetString(GL_VERSION);
        MessageBoxA(0, version, "OPEN GL VERSION", 0);

        glClearColor(0.129f, 0.586f, 0.949f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(realDC);

        return true;
    }

    /*protected*/ void Window::OnClose()
    {
        DestroyWindow(m_HWnd);
    }

}