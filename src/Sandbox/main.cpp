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

		m_CameraController = Engine::CameraController2D();
		m_CameraController.SetCamera(cam);
		m_CameraController.SetKeyDownFunc([&](Engine::KeyCode k) { return m_Window->IsKeyDown(k); });

		Engine::RenderCommand::EnableBlending(true);

		m_Tex.Bind(0);
		m_Tex.SetImage("res/images/apple.jpg");

		m_Tex2.Bind(0);
		m_Tex2.SetImage("res/images/cam.png");
	}

	virtual void OnUpdate(float timestep) override
	{
		m_CameraController.OnUpdate(timestep);

		//Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
		//Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f });
		//Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.5f, 0.2f, 0.5f }, { 0.2f, 0.2f });

		//float aspect = (float)m_Tex.GetWidth() / (float)m_Tex.GetHeight();
		//float height = 2;
		//Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { height * aspect, height }, { 0.0f, 0.0f }, m_Tex);
		////Engine::Renderer2D::DrawQuad({ -3.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, m_Tex);

		//Engine::Renderer2D::EndScene();

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				if ((i + j) % 2 == 0)
				{
					//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, { i / 50.0f, j / 50.0f, 0.3f, 1.0f }, { 0.0f, 0.0f });
					Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, m_Tex, { 0.0f, 0.0f });
				}
				else
				{
					//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, { i / 50.0f, j / 50.0f, 0.3f, 1.0f }, { 0.0f, 0.0f });
					Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, m_Tex2, { 0.0f, 0.0f });
				}
			}
		}
		Engine::Renderer2D::EndScene();
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


