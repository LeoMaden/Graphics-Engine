#pragma once

#include <Windows.h>

namespace Engine {

	class Window
	{
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		bool ShouldClose = false;

		bool Create();
		bool CreateContext();

		void PollEvents();
		void SwapBuffers();

	protected:
		void OnClose();

	private:
		HWND m_HWnd;
	};

}