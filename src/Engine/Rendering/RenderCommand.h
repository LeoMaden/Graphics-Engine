#pragma once

#include <glm/glm.hpp>

namespace Engine {

	class RenderCommand
	{
	public:
		static void Clear();
		static void Clear(const glm::vec4& color);

		static void EnableBlending(bool setting = true);
		static void SetViewport(const glm::vec2& size);

		static void SmoothLines(bool setting = true);
		static void LineWidth(float thickness);

		static void PointSize(float size);
	};

}