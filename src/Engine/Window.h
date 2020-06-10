#pragma once

#include <Windows.h>

#include "Events/Event.h"
#include "Events/KeyEvents.h"

namespace Engine {

	class Window
	{
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		bool Create();
		bool CreateContext();

		void SetCallback(Callback callback)		{ m_Callback = callback; }
		bool ShouldClose() const				{ return m_ShouldClose; }

		void PollEvents();
		void SwapBuffers();

		void OnEvent(Event& e) { m_Callback(e); }

	protected:
		void OnClose();


	private:
		bool		m_ShouldClose		= false;
		HWND		m_HWnd				= nullptr;
		Callback	m_Callback			= nullptr;
	};

}