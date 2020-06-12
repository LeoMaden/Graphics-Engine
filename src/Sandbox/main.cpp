#include "Application.h"
#include "Log.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

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

		//Engine::RenderCommand::EnableBlending();

		m_Tex.Bind(0);
		m_Tex.SetImage("res/images/apple2.png");

		m_Tex2.Bind(0);
		m_Tex2.SetImage("res/images/cam.png");
	}

	virtual void OnUpdate(float timestep) override
	{
		m_CameraController.OnUpdate(timestep);

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f });
		Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.5f, 0.2f, 0.5f }, { 0.2f, 0.2f });

		Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, m_Tex);
		Engine::Renderer2D::DrawQuad({ -3.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, m_Tex);

		Engine::Renderer2D::EndScene();

		//Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (int i = 0; i < 50; i++)
		//{
		//	for (int j = 0; j < 50; j++)
		//	{
		//		Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		//	}
		//}
		//Engine::Renderer2D::EndScene();
	}

private:
	Engine::CameraController2D	m_CameraController;
	Engine::Texture2D			m_Tex;
	Engine::Texture2D			m_Tex2;
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


