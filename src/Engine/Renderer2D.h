#pragma once
#include "OpenGL/Shader.h"

#include <memory>

namespace Engine {

	/*static*/ class Renderer2D
	{
	public:
		static void Init();
		static void DrawSquare();

	private:
		static std::unique_ptr<Shader>	s_FlatColorShader;
		static std::vector<float>		s_UnitSquarePositions;
	};

}