#include "Application.h"
#include "Log.h"

namespace Engine {

#define BIND_EVENT_FUNC(handler, type) [this](type& e) { this->handler(e); }

	Application::Application()
	{
		Log::Init();
		LOG_DEBUG("App constructor");

		m_Window = new Window();

		m_Window->Create();
		m_Window->CreateContext();
		m_Window->SetCallback(BIND_EVENT_FUNC(OnEvent, Event));

	}

	Application::~Application()
	{
		LOG_DEBUG("App destructor");

		delete m_Window;
	}

	void Application::OnEvent(Event& e)
	{
		DispatchEvent<KeyDownEvent>(e, BIND_EVENT_FUNC(OnKeyDown, KeyDownEvent));
	}

	void Application::Run()
	{
		LOG_DEBUG("App run");
		OnStartup();

		while (!m_Window->ShouldClose())
		{
			OnUpdate(0.0f);

			m_Window->SwapBuffers();
			m_Window->PollEvents();
		}
	}

	void Application::OnKeyDown(KeyDownEvent& e)
	{
		LOG_DEBUG("Key down: {}", e.GetKeyCode());
		switch (e.GetKeyCode())
		{
		case KeyCode::Escape:
			m_Window->Close();
			break;
		}
	}

}