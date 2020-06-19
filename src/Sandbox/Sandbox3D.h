#pragma once
#include "Application.h"
#include "Log.h"

#include "Rendering/Renderer2D.h"
#include "Rendering/RenderCommand.h"

#include "Camera/CameraControllerFPS.h"

class Sandbox3D : public Engine::Application
{
public:
	virtual void OnStartup() override
	{
		LOG_DEBUG("App startup");
		Engine::RenderCommand::InitRenderer();
		Engine::Renderer2D::Init();

		m_CameraController = Engine::CameraControllerFPS();
		m_CameraController.SetCamera(Engine::Camera3D());
		m_CameraController.SetKeyDownFunc([&](Engine::KeyCode k) { return m_Window->IsKeyDown(k); });

		Engine::RenderCommand::EnableBlending(true);
		Engine::RenderCommand::SmoothLines(true);
		Engine::RenderCommand::PointSize(5);
	}

	virtual void OnUpdate(float timestep) override
	{
		//Engine::RenderCommand::Clear({ 1, 1, 1, 1 });
		m_CameraController.OnUpdate(timestep);

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Engine::Renderer2D::DrawCircle({ 2,2 }, 1.0f, { 1,0,0,1 }, 25);
		Engine::Renderer2D::DrawQuad({ -2,2 }, { 1, 1 }, { 0,0,1,1 });
		Engine::Renderer2D::DrawLine({ 0,0 }, { 1,0 }, { 1, 0, 1, 1 });
		Engine::Renderer2D::DrawLine({ 0,0.5 }, { 1,1 }, { 1, 0, 1, 1 });
		Engine::Renderer2D::DrawPoint({ -1,0 }, { 1, 1, 0, 1 });

		Engine::Renderer2D::EndScene();

		//Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (int i = 0; i < 20; i++)
		//{
		//	for (int j = 0; j < 20; j++)
		//	{
		//		Engine::Renderer2D::DrawCircle({ i, j }, 0.45f, { i / 20.0, j / 20.0, 0.5f, 1 }, 20);
		//		//if ((i + j) % 2 == 0)
		//		//{
		//		//	//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, { i / 50.0f, j / 50.0f, 0.3f, 1.0f }, { 0.0f, 0.0f });
		//		//	//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, *m_GrassTex, { 0.0f, 0.0f });
		//		//}
		//		//else
		//		//{
		//		//	//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, { i / 50.0f, j / 50.0f, 0.3f, 1.0f }, { 0.0f, 0.0f });
		//		//	//Engine::Renderer2D::DrawQuad({ i, j }, { 0.9f, 0.9f }, *m_StoneTex, { 0.0f, 0.0f });
		//		//}
		//	}
		//}
		//Engine::Renderer2D::EndScene();
	}

	virtual void OnMouseMove(Engine::MouseMoveEvent& e) override
	{
		m_CameraController.OnMouseMove(e);
	}

	virtual void OnMouseScroll(Engine::MouseScrollEvent& e) override
	{
		m_CameraController.OnMouseScroll(e);
	}

	virtual void OnWindowResize(Engine::WindowResizeEvent& e) override
	{
		m_CameraController.OnWindowResize(e);
	}

private:
	Engine::CameraControllerFPS	m_CameraController;
};