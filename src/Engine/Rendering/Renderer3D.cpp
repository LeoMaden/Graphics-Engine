#include "Renderer3D.h"
#include "RenderCommand.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include <vector>

namespace Engine {

	struct ColorVertex {
		glm::vec3 Position;
		glm::vec4 Color;
	};

	using IndexType = uint16_t;


	struct Renderer3DData
	{
		std::vector<glm::vec3>		UnitCubePositions;
		std::vector<IndexType>		UnitCubeIndices;

		VertexBuffer*	VBO		= nullptr;
		IndexBuffer*	IBO		= nullptr;
		VertexArray*	VAO		= nullptr;
		Shader*			Shader	= nullptr;

		glm::mat4		SceneViewProjMat;
	};

	static Renderer3DData s_Data3D;



	void Renderer3D::Init()
	{
		s_Data3D.UnitCubePositions.resize(8);
		s_Data3D.UnitCubePositions = {
			{ 0, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }
		};

		s_Data3D.UnitCubeIndices.resize(36);
		s_Data3D.UnitCubeIndices = {
			0, 1, 2, 0, 2, 3, // Front
			4, 0, 3, 4, 3, 7, // Left
			1, 5, 6, 1, 6, 2, // Right
			3, 2, 6, 3, 6, 7, // Top
			4, 5, 1, 4, 1, 0, // Bottom
			5, 4, 7, 5, 7, 6  // Back
		};

		s_Data3D.VBO = new VertexBuffer(1000);
		s_Data3D.VBO->AddLayout(0, DataType::Float32, 3);
		s_Data3D.VBO->AddLayout(1, DataType::Float32, 4);

		s_Data3D.IBO = new IndexBuffer(10000);

		s_Data3D.VAO = new VertexArray(*s_Data3D.VBO, *s_Data3D.IBO);

		s_Data3D.Shader = new Shader();
		s_Data3D.Shader->AddVertexShader("res/shaders/FlatColor.vert");
		s_Data3D.Shader->AddFragmentShader("res/shaders/FlatColor.frag");
		s_Data3D.Shader->Link(); 
	}

	void Renderer3D::BeginScene(const Camera& camera)
	{
		s_Data3D.SceneViewProjMat = camera.GetViewProjMat();

		s_Data3D.Shader->Bind();
		s_Data3D.Shader->SetMat4("u_Transform", s_Data3D.SceneViewProjMat);
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		DrawCube(position, size, color, size / 2.0f);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& centre)
	{
		ColorVertex vertices[8];
		IndexType indices[36];

		for (uint64_t i = 0; i < 8; i++)
		{
			vertices[i].Position = s_Data3D.UnitCubePositions[i] * size - centre + position;
			vertices[i].Color = color;
		}

		for (uint64_t i = 0; i < 36; i++)
		{
			indices[i] = s_Data3D.UnitCubeIndices[i];
		}

		s_Data3D.Shader->Bind();

		s_Data3D.VAO->Bind();

		s_Data3D.VBO->Bind();
		s_Data3D.VBO->SetData(vertices, sizeof(vertices));

		s_Data3D.IBO->Bind();
		s_Data3D.IBO->SetIndices(indices, sizeof(indices));

		RenderCommand::DrawIndexed(DrawMode::Triangles, 36, DataType::UInt16);
	}

}