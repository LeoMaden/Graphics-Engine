#include "Renderer2D.h"
#include "Log.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	/*static*/ std::unique_ptr<Shader>	Renderer2D::s_FlatColorShader;
	/*static*/ std::vector<float>		Renderer2D::s_UnitSquarePositions;
	/*static*/ glm::mat4				Renderer2D::s_SceneViewProjMat(1.0f);

	/*static*/ void Renderer2D::Init()
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) 
		{
			LOG_INFO("Open GL Debug enabled");
			
			auto callback = [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				if (severity == GL_DEBUG_SEVERITY_HIGH)
				{
					LOG_ERROR("Open GL: {}", message);
				}
				else
				{
					LOG_WARN("Open GL: {}", message);
				}
			};

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(callback, nullptr);
		}
		else
		{
			LOG_INFO("Open GL Debug not enabled");
		}

		s_UnitSquarePositions = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		std::vector<uint32_t> indices{
			0, 1, 2, 2, 3, 0
		};

		VertexBuffer vbo(s_UnitSquarePositions);
		vbo.AddLayout(0, GL_FLOAT, 3);

		IndexBuffer ibo(indices);

		VertexArray vao(vbo, ibo);

		s_FlatColorShader = std::make_unique<Shader>();
		s_FlatColorShader->AddVertexShader("res/shaders/FlatColor.vert");
		s_FlatColorShader->AddFragmentShader("res/shaders/FlatColor.frag");

		s_FlatColorShader->Link();
		s_FlatColorShader->Bind();

		s_FlatColorShader->SetVec3("uColor", { 1.0f, 0.0f, 0.0f });
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		s_FlatColorShader->SetMat4("u_Transform", s_SceneViewProjMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_SceneViewProjMat = camera.GetViewProjMat();
	}

}