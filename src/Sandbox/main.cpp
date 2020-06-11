#include "Application.h"
#include "Log.h"
#include "Renderer2D.h"

#include "Camera/Camera2D.h"

#include <iostream>

class SandboxApp : public Engine::Application
{
public:
	virtual void OnStartup() override
	{
		LOG_DEBUG("App startup");
		Engine::Renderer2D::Init();

		m_Camera.SetProjection({ 16, 9 }, { 8, 4.5f });
	}

	virtual void OnUpdate(float timestep) override
	{
		float speed = 0.1f;

		bool wDown = m_Window->IsKeyDown(Engine::KeyCode::W);
		bool sDown = m_Window->IsKeyDown(Engine::KeyCode::S);

		if (wDown)
		{
			m_Camera.Move({ 0.0f, -timestep * speed, 0.0f });
		}

		if (sDown)
		{
			m_Camera.Move({ 0.0f, timestep * speed, 0.0f });
		}


		Engine::Renderer2D::BeginScene(m_Camera);
		Engine::Renderer2D::DrawSquare();
	}

private:
	Engine::Camera2D m_Camera;
};

int main()
{
	SandboxApp* app = new SandboxApp();

	app->Run();

	delete app;

	LOG_DEBUG("Main end");
	std::cin.get();
	return 0;
}


