#include "Renderer3D.h"
#include "RenderCommand.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include <vector>
#include <glm/gtc/constants.hpp>

namespace Engine {

	struct ColorVertex3D {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 Normal;
	};

	using IndexType = uint16_t;


	struct Renderer3DData
	{
		std::vector<glm::vec3>		UnitCubePositions;
		std::vector<glm::vec3>		UnitCubeNormals;
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
		s_Data3D.UnitCubePositions.resize(24);
		s_Data3D.UnitCubePositions = {
			{ 0, 0, 0 }, // 1
			{ 1, 0, 0 },
			{ 1, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 }, // 2
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 1 },
			{ 0, 0, 1 }, // 3
			{ 1, 0, 1 },
			{ 1, 0, 0 },
			{ 0, 0, 0 },
			{ 0, 1, 1 }, // 4
			{ 1, 1, 1 },
			{ 1, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 }, // 5
			{ 0, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 0, 1 }, // 6
			{ 1, 0, 0 },
			{ 1, 1, 0 },
			{ 1, 1, 1 }
		};

		s_Data3D.UnitCubeNormals.resize(6);
		s_Data3D.UnitCubeNormals = {
			{ 0, 0, -1 }, // First 4 vertices
			{ 0, 0, 1 },
			{ 0, -1, 0 },
			{ 0, 1, 0 },
			{ -1, 0, 0 },
			{ 1, 0, 0 } // Last 4 vertices
		};

		s_Data3D.UnitCubeIndices.resize(36);
		s_Data3D.UnitCubeIndices = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		s_Data3D.VBO = new VertexBuffer(100000);
		s_Data3D.VBO->AddLayout(0, DataType::Float32, 3);
		s_Data3D.VBO->AddLayout(1, DataType::Float32, 4);
		s_Data3D.VBO->AddLayout(2, DataType::Float32, 3);

		s_Data3D.IBO = new IndexBuffer(17430);

		s_Data3D.VAO = new VertexArray(*s_Data3D.VBO, *s_Data3D.IBO);

		s_Data3D.Shader = new Shader();
		s_Data3D.Shader->AddVertexShader("res/shaders/FlatColor3D.vert");
		s_Data3D.Shader->AddFragmentShader("res/shaders/FlatColor3D.frag");
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
		ColorVertex3D vertices[24];
		IndexType indices[36];

		for (uint64_t i = 0; i < 24; i++)
		{
			vertices[i].Position = s_Data3D.UnitCubePositions[i] * size - centre + position;
			vertices[i].Color = color;
			vertices[i].Normal = s_Data3D.UnitCubeNormals[i / 4];
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

	void Renderer3D::DrawSphere(const glm::vec3& centre, float radius, const glm::vec4& color, uint32_t nDivisions)
	{
		std::vector<ColorVertex3D> vertices;
		std::vector<IndexType> indices;

		uint32_t nSegments = nDivisions;
		uint32_t nStacks = nDivisions - 1;

		double theta = 0.0; // X axis
		double phi = 0.0;   // Z axis
		double dTheta = glm::two_pi<double>() / (double)(nSegments);
		double dPhi = glm::pi<double>() / (double)(nStacks);

		for (uint64_t j = 0; j < (nStacks + 1); j++)
		{
			for (uint64_t i = 0; i < nSegments; i++)
			{
				glm::vec3 norm = {
					glm::sin(phi)* glm::cos(theta),
					glm::sin(phi)* glm::sin(theta),
					glm::cos(phi)
				};

				glm::vec3 pos = centre * radius + norm;

				vertices.push_back({ pos, color, norm });

				theta += dTheta;
			}
			theta = 0.0;
			phi += dPhi;
		}

		for (uint32_t j = 0; j < nStacks; j++)
		{
			for (uint32_t i = 0; i < nSegments; i++)
			{
				uint32_t segi = i;
				uint32_t nextSegi = (i + 1) % nSegments;

				uint32_t stacki = j;
				uint32_t nextStacki = j + 1;

				uint32_t i0 = segi +		(nSegments * stacki);
				uint32_t i1 = segi +		(nSegments * nextStacki);
				uint32_t i2 = nextSegi +	(nSegments * nextStacki);
				uint32_t i3 = nextSegi +	(nSegments * stacki);

				// Two triangles per sector except first and last stacks
				if (j != 0)
				{
					indices.push_back(i0);
					indices.push_back(i2);
					indices.push_back(i3);
				}

				if (j != (nStacks - 1))
				{
					indices.push_back(i0);
					indices.push_back(i1);
					indices.push_back(i2);
				}
			}
		}

		//for (int i = 0; i < vertices.size(); i++)
		//{
		//	indices.push_back(i);
		//}


		s_Data3D.Shader->Bind();

		s_Data3D.VAO->Bind();

		s_Data3D.VBO->Bind();
		s_Data3D.VBO->SetData(vertices.data(), vertices.size() * sizeof(ColorVertex3D));

		s_Data3D.IBO->Bind();
		s_Data3D.IBO->SetIndices(indices.data(), indices.size() * sizeof(IndexType));

		RenderCommand::DrawIndexed(DrawMode::Triangles, indices.size(), DataType::UInt16);

	}

}