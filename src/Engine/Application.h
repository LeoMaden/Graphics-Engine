#pragma once
#include "Window.h"
#include "Events/Event.h"

namespace Engine {

	class Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);
		void Run();

		virtual void OnStartup() {}
		virtual void OnUpdate(float timestep) {}

	protected:
		void OnKeyDown(KeyDownEvent& e);

		Window* m_Window;
	};

}