#pragma once
#include "OpenGL/Shader.h"

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

	private:
		static std::unique_ptr<Shader>	s_FlatColorShader;
		static std::vector<float>		s_UnitSquarePositions;
		static glm::mat4				s_SceneViewProjMat;
	};

}