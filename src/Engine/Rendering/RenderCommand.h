#pragma once

#include <glm/glm.hpp>

#include "RenderUtils.h"

namespace Engine {

	class RenderCommand
	{
	public:
		static void InitRenderer();

		static void DrawIndexed(DrawMode mode, uint32_t count, DataType type = DataType::UInt32, uint32_t offset = 0);

		static void Clear();
		static void Clear(const glm::vec4& color);

		static void EnableBlending(bool setting = true);
		static void EnableDepth(bool setting = true);
		static void SetViewport(const glm::vec2& size);

		static void SmoothLines(bool setting = true);
		static void LineWidth(float thickness);

		static void PointSize(float size);
	};

}