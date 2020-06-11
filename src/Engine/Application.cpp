#include "Application.h"
#include "Log.h"
#include "RenderCommand.h"

#include "Renderer2D.h"

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

		m_LastFrameTime = std::chrono::high_resolution_clock::now();

		while (!m_Window->ShouldClose())
		{
			TimePoint current = std::chrono::high_resolution_clock::now();
			float timestep = (current - m_LastFrameTime).count() / 1e9;
			m_LastFrameTime = current;

			//LOG_TRACE("Frame time {} ({} fps)", timestep, 1.0f / timestep);

			RenderCommand::Clear();

			OnUpdate(timestep);

			LOG_TRACE("FPS: {:.0f}, Draws: {}, Quads: {}", 1.0f / timestep, Renderer2D::Stats.Draws, Renderer2D::Stats.Quads);
			Renderer2D::Stats.Draws = 0;
			Renderer2D::Stats.Quads = 0;

			m_Window->SwapBuffers();
			m_Window->PollEvents();
			//Sleep(15);
		}
	}

	void Application::OnKeyDown(KeyDownEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case KeyCode::Escape:
			m_Window->Close();
			break;
		}
	}

}