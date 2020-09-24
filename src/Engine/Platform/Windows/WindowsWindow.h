#pragma once
#include "Core/Window.h"

#include "Platform/OpenGL/OpenGLContextProperties.h"

#include <Windows.h>

namespace Engine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(Window::Properties props);
		~WindowsWindow();

		virtual void CreateOpenGLContext(OpenGLContextProperties props) override;

		virtual void SetCallback(CallbackFunc func) override { m_Callback = func; }

		virtual void SwapBuffers() const override;
		virtual void PollEvents() const override;

		virtual void Show() const override;
		virtual void Hide() const override;

		virtual bool IsOpen() const override { return m_IsOpen; }
		virtual Vector2 GetClientSize() const override;

	// Window proc handler functions.
	public:
		void Close();

	private:
		HWND m_WindowHandle;	// Handle to this window.
		HDC m_DeviceContextHandle;

		CallbackFunc m_Callback;
		bool m_IsOpen = true;
	};

}