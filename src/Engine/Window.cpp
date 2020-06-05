#include "Window.h"

#include <iostream>

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
            wc.lpszClassName,               // Window class
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
        // Device context.
        HDC hdc = GetDC(m_HWnd);

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        int format = ChoosePixelFormat(hdc, &pfd);
        if (format == 0)
        {
            return false;       // Pixel format invalid.
        }
        SetPixelFormat(hdc, format, &pfd);

        // OpenGL rendering context.
        HGLRC glContext = wglCreateContext(hdc);
        BOOL ok = wglMakeCurrent(hdc, glContext);
        if (!ok)
        {
            return false;       // OpenGL context not created.
        }
        return true;
    }

    /*protected*/ void Window::OnClose()
    {
        DestroyWindow(m_HWnd);
    }

}