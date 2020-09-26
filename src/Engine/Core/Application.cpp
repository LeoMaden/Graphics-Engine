#include "Pch.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/CurrentContext.h"

namespace Engine {

	/*static*/ void Application::Start()
	{
		s_Application->OnStartup();
		s_Application->Run();
	}

	/*static*/ void Application::Destroy()
	{
		s_Application->OnDestruction();
		delete s_Application;
		s_Application = nullptr;
	}

	/*static*/ Application* Application::Get()
	{
		return s_Application;
	}

	/*static*/ Application* Application::s_Application;


	Application::Application(const ApplicationProperties& props)
	{
		// If no window properties provided, use default.
		if (props.WindowProps == nullptr)
		{
			Window = Window::Create(WindowProperties());
		}
		else
		{
			Window = Window::Create(*props.WindowProps);
		}

		Window->CreateRenderContext(*props.ContextProps);
		Window->SetCallback([&](Event& e) { OnEvent(e); });
		
		Input::SetProvider(Window);
	}

	Application::~Application()
	{
		Window->DeleteRenderContext(CurrentContext::Get());
		delete Window;
	}

	// Override-able protected functions.
	void Application::OnCreation()
	{
	}

	void Application::OnStartup()
	{
	}

	void Application::OnUpdate(float ts)
	{
	}

	void Application::OnDestruction()
	{
	}

	void Application::OnEvent(Event& e)
	{
	}


	void Application::Run()
	{
		while (Window->IsOpen())
		{
			OnUpdate(0);

			Window->PollEvents();
			Window->SwapBuffers();
		}
	}

}