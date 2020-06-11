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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		
	private:
		static std::unique_ptr<Shader>	s_FlatColorShader;
		static std::vector<float>		s_UnitSquarePositions;
		static glm::mat4				s_SceneViewProjMat;

		static VertexBuffer*			s_VBO;
		static IndexBuffer*				s_IBO;
		static VertexArray*				s_VAO;
	};

}