#pragma once
#include "Application.h"
#include "Log.h"

#include "Rendering/Renderer2D.h"
#include "Rendering/Renderer3D.h"
#include "Rendering/RenderCommand.h"

#include "Camera/CameraControllerFPS.h"
#include "Camera/CameraController3rdPerson.h"
#include "Utils.h"

class Sandbox3D : public Engine::Application
{
public:
	virtual void OnStartup() override
	{
		LOG_DEBUG("App startup");
		Engine::RenderCommand::InitRenderer();
		Engine::Renderer2D::Init();
		Engine::Renderer3D::Init();

		//m_CameraController = Engine::CameraController3rdPerson();
		m_CameraController = Engine::CameraControllerFPS();
		m_CameraController.SetCamera(Engine::Camera3D());
		//m_CameraController.SetTarget(glm::vec3(0));
		m_CameraController.SetKeyDownFunc([&](Engine::KeyCode k) { return m_Window->IsKeyDown(k); });

		Engine::RenderCommand::EnableBlending(true);
		Engine::RenderCommand::EnableDepth(true);
		Engine::RenderCommand::SmoothLines(true);
		Engine::RenderCommand::PointSize(5);

		m_TestScene = Engine::Utils::LoadScene("C:/Users/leocm/Downloads/test-model/cube.obj");
		m_TestScene->Name = "Test";
	}

	virtual void OnUpdate(float timestep) override
	{
		using namespace Engine;

		m_CameraController.OnUpdate(timestep);

		PointLight pl;
		pl.Position = glm::vec3(2, 2.5, 3);

		pl.Ambient = glm::vec3(0.2);
		pl.Diffuse = glm::vec3(1.0);
		pl.Specular = glm::vec3(1.0);

		pl.Constant = 1;
		pl.Linear = 0.09;
		pl.Quadratic = 0.032;

		Lighting l;
		l.PointLights.push_back(pl);


		Renderer3D::BeginScene(m_CameraController.GetCamera(), l);

		Renderer3D::DrawMesh(*m_TestScene->Meshes[0]);

		Renderer3D::EndScene();
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
	//Engine::CameraController3rdPerson	m_CameraController;
	Engine::Scene* m_TestScene;
};