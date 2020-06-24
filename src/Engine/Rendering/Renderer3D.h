#pragma once
#include "Camera/Camera.h"

#include <glm/glm.hpp>

namespace Engine {

	class Renderer3D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& centre);

		static void DrawSphere(const glm::vec3& centre, float radius, const glm::vec4& color, uint32_t nDivisions);
	};

}