#include "Renderer2D.h"
#include "Log.h"

#include "OpenGL/Shader.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	static struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	using IndexType = uint16_t;

	struct Renderer2DData
	{
		std::unique_ptr<Shader>	FlatColorShader;
		glm::vec3				UnitSquarePositions[4];
		glm::mat4				SceneViewProjMat;

		VertexBuffer* VBO;
		IndexBuffer* IBO;
		VertexArray* VAO;

		uint32_t QuadsPerBatch = 1000;
		uint32_t QuadCount = 0;

		uint32_t VBOSize = 4 * QuadsPerBatch * sizeof(QuadVertex);
		uint32_t IBOSize = 6 * QuadsPerBatch * sizeof(IndexType);

		byte* BatchData;
		byte* BatchDataInsPtr;
		uint32_t VertexCount = 0;

		byte* BatchIndices;
		byte* BatchIndicesInsPtr;
		uint32_t IndexCount = 0;
	};

	static Renderer2DData s_Data;

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

		s_Data.VBO = new VertexBuffer(s_Data.VBOSize);
		s_Data.VBO->AddLayout(0, GL_FLOAT, 3); // Position
		s_Data.VBO->AddLayout(1, GL_FLOAT, 4); // Color

		s_Data.IBO = new IndexBuffer(s_Data.IBOSize);

		s_Data.VAO = new VertexArray(*s_Data.VBO, *s_Data.IBO);

		s_Data.FlatColorShader = std::make_unique<Shader>();
		s_Data.FlatColorShader->AddVertexShader("res/shaders/FlatColor.vert");
		s_Data.FlatColorShader->AddFragmentShader("res/shaders/FlatColor.frag");

		s_Data.FlatColorShader->Link();
		s_Data.FlatColorShader->Bind();

		s_Data.BatchData = new byte[s_Data.VBOSize];
		s_Data.BatchIndices = new byte[s_Data.IBOSize];
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		s_Data.FlatColorShader->SetMat4("u_Transform", s_Data.SceneViewProjMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.SceneViewProjMat = camera.GetViewProjMat();
		s_Data.FlatColorShader->SetMat4("u_Transform", s_Data.SceneViewProjMat);

		s_Data.QuadCount = 0;
		s_Data.VertexCount = 0;
		s_Data.IndexCount = 0;

		s_Data.BatchDataInsPtr = s_Data.BatchData;
		s_Data.BatchIndicesInsPtr = s_Data.BatchIndices;
	}

	void Renderer2D::EndScene()
	{
		s_Data.VBO->SetData(s_Data.BatchData, s_Data.VertexCount * sizeof(QuadVertex));
		s_Data.IBO->SetIndices(s_Data.BatchIndices, s_Data.IndexCount * sizeof(IndexType));

		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_SHORT, 0);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		QuadVertex vertices[4];
		IndexType indices[6];

		glm::vec3 pos3 = glm::vec3(position, 0.0f);
		glm::vec3 size3 = glm::vec3(size, 0.0f);

		indices[0] = 0 + 4 * s_Data.QuadCount;
		indices[1] = 1 + 4 * s_Data.QuadCount;
		indices[2] = 2 + 4 * s_Data.QuadCount;
		indices[3] = 2 + 4 * s_Data.QuadCount;
		indices[4] = 3 + 4 * s_Data.QuadCount;
		indices[5] = 0 + 4 * s_Data.QuadCount;

		vertices[0].Position = s_Data.UnitSquarePositions[0] * size3 + pos3;
		vertices[1].Position = s_Data.UnitSquarePositions[1] * size3 + pos3;
		vertices[2].Position = s_Data.UnitSquarePositions[2] * size3 + pos3;
		vertices[3].Position = s_Data.UnitSquarePositions[3] * size3 + pos3;

		vertices[0].Color = color;
		vertices[1].Color = color;
		vertices[2].Color = color;
		vertices[3].Color = color;

		memcpy(s_Data.BatchDataInsPtr, vertices, sizeof(vertices));
		memcpy(s_Data.BatchIndicesInsPtr, indices, sizeof(indices));

		s_Data.QuadCount++;
		s_Data.VertexCount += 4;
		s_Data.IndexCount += 6;

		s_Data.BatchDataInsPtr += 4 * sizeof(QuadVertex);
		s_Data.BatchIndicesInsPtr += 6 * sizeof(IndexType);
	}

}