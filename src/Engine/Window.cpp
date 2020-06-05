#include "Window.h"

#include <iostream>

namespace Engine {

    /*static*/ LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		const wchar_t WND_CLASS[] = L"Main Window";

        WNDCLASS wc = { };

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = NULL;
        wc.lpszClassName = WND_CLASS;

        RegisterClass(&wc);

        // Create the window.
        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            WND_CLASS,                      // Window class
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

    /*protected*/ void Window::OnClose()
    {
        DestroyWindow(m_HWnd);
    }

}