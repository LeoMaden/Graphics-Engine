#pragma once
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Texture2DBase.h"

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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& centre);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture, const glm::vec2& centre);

		static void DrawCircle(const glm::vec2& centre, float radius, const glm::vec4& color, uint32_t nDivisions);

		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

		static void DrawPoint(const glm::vec2& position, const glm::vec4& color);

		struct Statistics
		{
			uint32_t Draws = 0;
			uint32_t Quads = 0;
			uint32_t Circles = 0;
			uint32_t Lines = 0;
			uint32_t Points = 0;
			uint32_t Textures = 0;

			void Reset()
			{
				Draws = 0;
				Quads = 0;
				Circles = 0;
				Lines = 0;
				Points = 0;
				Textures = 0;
			}
		};

		static Statistics Stats;
	};

}