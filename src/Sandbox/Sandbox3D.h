#pragma once
#include "Application.h"
#include "Log.h"

#include "Rendering/Renderer2D.h"
#include "Rendering/Renderer3D.h"
#include "Rendering/RenderCommand.h"

#include "Camera/CameraControllerFPS.h"
#include "Camera/CameraController3rdPerson.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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


		m_CameraController = Engine::CameraController3rdPerson();
		m_CameraController.SetCamera(Engine::Camera3D());
		m_CameraController.SetTarget(glm::vec3(0));
		//m_CameraController.SetKeyDownFunc([&](Engine::KeyCode k) { return m_Window->IsKeyDown(k); });

		Engine::RenderCommand::EnableBlending(true);
		Engine::RenderCommand::EnableDepth(true);
		Engine::RenderCommand::SmoothLines(true);
		Engine::RenderCommand::PointSize(5);

		Assimp::Importer aiImporter;
		const aiScene* aiScene = aiImporter.ReadFile("C:/Users/leocm/Downloads/tkf28u03u0ow-3dbuch/3dbuch/book.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

		Engine::Scene* scene = Engine::Utils::LoadScene("C:/Users/leocm/Downloads/tkf28u03u0ow-3dbuch/3dbuch/book.obj");
		scene->Name = "Book";
	}

	virtual void OnUpdate(float timestep) override
	{
		using namespace Engine;

		m_CameraController.OnUpdate(timestep);

		Lighting lights;
		DirectionalLight dirLight;
		dirLight.Direction = glm::normalize(glm::vec3(1, -1, 0));
		dirLight.Ambient = glm::vec3(0.05f);
		dirLight.Diffuse = glm::vec3(0.5f);
		dirLight.Specular = glm::vec3(0.8f);

		lights.DirectionalLights.push_back(dirLight);

		Renderer3D::BeginScene(m_CameraController.GetCamera(), lights);
		Renderer3D::DrawSphere({ 0,0,0 }, 1.0f, { 1, 0, 1, 1 }, 20);
		Renderer3D::DrawCube({ 3,3,3}, { 1, 1, 1 }, { 0, 1, 0, 1 });
		Renderer3D::DrawCube({ 0,5,0}, { 0.1, 0.1, 0.1 }, { 1, 0, 0, 1 });
		//Renderer3D::DrawCube({ 0,0,0 }, { 2, 2, 1 }, { 1, 0, 1, 1 }, { 0,0,0 });
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
	//Engine::CameraControllerFPS	m_CameraController;
	Engine::CameraController3rdPerson	m_CameraController;
};