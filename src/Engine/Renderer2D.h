#pragma once
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"

#include "Camera/Camera.h"

#include <memory>

#include <glm/glm.hpp>

namespace Engine {

	/*static*/ class Renderer2D
	{
	public:
		static void Init();
		static void DrawSquare();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void FlushAndReset();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& centre);
		

		struct Statistics
		{
			uint32_t Draws = 0;
			uint32_t Quads = 0;

			void Reset()
			{
				Draws = 0;
				Quads = 0;
			}
		};

		static Statistics Stats;
	};

}