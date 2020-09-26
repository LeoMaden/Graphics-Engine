#pragma once
#include "Core/ApplicationProperties.h"
#include "Core/Window.h"

namespace Engine {

	class Application
	{
	// Static methods.
	public:
		template<typename T>
		static Application* Create(const ApplicationProperties& props)
		{
			s_Application = new T(props);
			s_Application->OnCreation();
			return s_Application;
		}

		static void Start();
		static void Destroy();

		static Application* Get();

	private:
		static Application* s_Application;

	// Instance members.
	public:
		Application(const ApplicationProperties& props);
		~Application();

	protected:
		// OnCreation is called once when Application::Create is called.
		// It runs after the constructor and window & graphics context are created.
		virtual void OnCreation();
		// OnStartup is called once when Application::Start is called.
		virtual void OnStartup();
		// OnUpdate is called every iteration of the main loop ie
		// once per frame.
		virtual void OnUpdate(float ts);
		// OnDestruction is called when Application::Destroy is called.
		// It runs before the destructor is called.
		virtual void OnDestruction();

		virtual void OnEvent(Event& e);

		Window* Window;

	private:
		void Run();

	};

}