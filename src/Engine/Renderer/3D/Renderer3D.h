#pragma once
#include "Camera/Camera.h"
#include "Lights/Lighting.h"

#include <glm/glm.hpp>

#include "Scene/Scene.h"
#include "OpenGL/Shader.h"

namespace Engine {

	class Renderer3D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera);
		static void BeginScene(const Camera& camera, const Lighting& lights);

		static void EndScene();

		static void DrawScene(const Scene& scene, const glm::mat4& transform);
		static void DrawNode(const Node& node, const glm::mat4& parentTransform);
		static void DrawMesh(const Mesh& mesh, const glm::mat4& transform);



		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& centre);

		static void DrawSphere(const glm::vec3& centre, float radius, const glm::vec4& color, uint32_t nDivisions);
	};

}