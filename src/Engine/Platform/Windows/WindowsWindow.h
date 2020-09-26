#pragma once
#include "Core/Window.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <Windows.h>
#include <windowsx.h>

namespace Engine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(Window::Properties props);
		~WindowsWindow();

		virtual RenderContext* CreateRenderContext(const RenderContextProperties& props) override;
		virtual void DeleteRenderContext(RenderContext* context) override;

		virtual void SetCallback(CallbackFunc func) override { m_Callback = func; }

		virtual void SwapBuffers() const override;
		virtual void PollEvents() const override;

		virtual void Show() const override;
		virtual void Hide() const override;

		virtual bool IsOpen() const override { return m_IsOpen; }
		virtual Vector2 GetClientSize() const override;

	// InputProvidor functions inherited from Window.
	public:
		virtual KeyState GetKeyState(KeyCode key) const override;
		virtual Vector2 GetMousePosition() const override;

	// Window proc handler functions.
	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void Close();
		void OnResize(LPARAM lParam);

		void OnKeyDown(WPARAM wParam, LPARAM lParam);
		void OnKeyUp(WPARAM wParam, LPARAM lParam);

		void OnMouse1Down(WPARAM wParam, LPARAM lParam);
		void OnMouseMove(WPARAM wParam, LPARAM lParam);
		void OnMouseScroll(WPARAM wParam, LPARAM lParam);

	// Window proc helper functions & data.
	private:
		static List<Pair<KeyCode, WPARAM>> m_KeyCodeConversion;
		static Map<KeyCode, WPARAM> m_WinKeyCodeMap;
		static Map<WPARAM, KeyCode> m_EngineKeyCodeMap;

		static WPARAM GetWinKeyCode(KeyCode code);
		static KeyCode GetEngineKeyCode(WPARAM winCode);
		static KeyFlags GetKeyFlags(LPARAM lParam);
		static ModifierKeys GetModKeys(WPARAM wParam);

	// Rendering API specific context creation.
	private:
		OpenGLContext* CreateOpenGLContext(const OpenGLContextProperties& props) const;
		void DeleteOpenGLContext(OpenGLContext* context) const;

	private:
		HWND m_WindowHandle;	// Handle to this window.
		HDC m_DeviceContextHandle;

		CallbackFunc m_Callback = [](Event&) {};
		bool m_IsOpen = true;
	};

}