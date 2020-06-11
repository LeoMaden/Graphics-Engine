#include "Renderer2D.h"
#include "Log.h"

#include "OpenGL/Shader.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	struct Renderer2DData
	{
		std::unique_ptr<Shader>	FlatColorShader;
		glm::vec3				UnitSquarePositions[4];
		glm::mat4				SceneViewProjMat;

		VertexBuffer* VBO;
		IndexBuffer* IBO;
		VertexArray* VAO;
	};

	static Renderer2DData s_Data;

	static struct QuadVertex
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	};

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

		s_Data.UnitSquarePositions[0] = { -0.5f, -0.5f, 0.0f };
		s_Data.UnitSquarePositions[1] = { 0.5f, -0.5f, 0.0f };
		s_Data.UnitSquarePositions[2] = { 0.5f,  0.5f, 0.0f };
		s_Data.UnitSquarePositions[3] = { -0.5f,  0.5f, 0.0f };

		uint16_t indices[] {
			0, 1, 2, 2, 3, 0
		};

		s_Data.VBO = new VertexBuffer(1024);
		s_Data.VBO->AddLayout(0, GL_FLOAT, 3);

		s_Data.IBO = new IndexBuffer(1024);
		s_Data.IBO->SetIndices(indices, sizeof(indices));

		s_Data.VAO = new VertexArray(*s_Data.VBO, *s_Data.IBO);

		s_Data.FlatColorShader = std::make_unique<Shader>();
		s_Data.FlatColorShader->AddVertexShader("res/shaders/FlatColor.vert");
		s_Data.FlatColorShader->AddFragmentShader("res/shaders/FlatColor.frag");

		s_Data.FlatColorShader->Link();
		s_Data.FlatColorShader->Bind();

		s_Data.FlatColorShader->SetVec3("uColor", { 1.0f, 0.0f, 0.0f });
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		s_Data.FlatColorShader->SetMat4("u_Transform", s_Data.SceneViewProjMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.SceneViewProjMat = camera.GetViewProjMat();
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		QuadVertex vertices[4];

		glm::vec3 pos3 = glm::vec3(position, 0.0f);
		glm::vec3 size3 = glm::vec3(size, 0.0f);


		vertices[0].Position = s_Data.UnitSquarePositions[0] * size3 + pos3;
		vertices[1].Position = s_Data.UnitSquarePositions[1] * size3 + pos3;
		vertices[2].Position = s_Data.UnitSquarePositions[2] * size3 + pos3;
		vertices[3].Position = s_Data.UnitSquarePositions[3] * size3 + pos3;

		s_Data.VBO->SetData(vertices, sizeof(vertices));

		s_Data.FlatColorShader->SetVec4("u_Color", color);
		s_Data.FlatColorShader->SetMat4("u_Transform", s_Data.SceneViewProjMat);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	}

}