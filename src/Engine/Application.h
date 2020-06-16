#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include <chrono>

namespace Engine {

	class Application
	{
		using TimePoint = std::chrono::steady_clock::time_point;
	public:
		Application();
		~Application();

		void OnEvent(Event& e);
		void Run();

		virtual void OnStartup() {}
		virtual void OnUpdate(float timestep) {}

	protected:
		virtual void OnKeyDown(KeyDownEvent& e);
		virtual void OnWindowResize(WindowResizeEvent& e);
		virtual void OnMouseScroll(MouseScrollEvent& e);

		Window*		m_Window;
		TimePoint	m_LastFrameTime;

	};

}