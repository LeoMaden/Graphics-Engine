#include "Application.h"
#include "Log.h"
#include "Renderer2D.h"

#include "Camera/CameraController2D.h"

#include <iostream>

class SandboxApp : public Engine::Application
{
public:
	virtual void OnStartup() override
	{
		LOG_DEBUG("App startup");
		Engine::Renderer2D::Init();

		Engine::Camera2D cam;
		cam.SetProjection({ 16, 9 }, { 8, 4.5f });

		m_CameraController.SetCamera(cam);
		m_CameraController.SetKeyDownFunc([&](Engine::KeyCode k) { return m_Window->IsKeyDown(k); });
	}

	virtual void OnUpdate(float timestep) override
	{
		m_CameraController.OnUpdate(timestep);

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		Engine::Renderer2D::DrawQuad({ 2.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
		Engine::Renderer2D::DrawQuad({ -2.0f, 0.0f }, { 1.0f, 2.0f }, { 1.0f, 0.6f, 0.2f, 1.0f });
	}

private:
	Engine::CameraController2D m_CameraController;
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


