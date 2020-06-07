#include "Renderer2D.h"
#include "Log.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	/*static*/ void Renderer2D::Setup()
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

		std::vector<float> vertices{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
		};
		std::vector<uint32_t> indices{
			0, 1, 2, 2, 3, 0
		};

		VertexBuffer vbo(vertices);
		vbo.AddLayout(0, GL_FLOAT, 3);
		vbo.AddLayout(1, GL_FLOAT, 3);

		IndexBuffer ibo(indices);

		VertexArray vao(vbo, ibo);

		Shader shader;
		shader.AddVertexShader("res/shaders/basic.vert");
		shader.AddFragmentShader("res/shaders/basic.frag");

		shader.Link();
		shader.Bind();
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}