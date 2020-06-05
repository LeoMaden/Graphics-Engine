#pragma once

#include <Windows.h>

namespace Engine {

	class Window
	{
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		bool Create();
		void RunSystemEventLoop();

	protected:
		void OnClose();

	private:
		HWND m_HWnd;
	};

}