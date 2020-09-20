#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Batch.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include <vector>
#include <sstream>
#include <glm/gtc/constants.hpp>

#include "Log.h"
#include "OpenGL\Texture2D.h"

namespace Engine {

	struct ColorVertex3D {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 Normal;
	};

	using IndexType = uint16_t;


	struct Renderer3DData
	{
		glm::mat4		SceneViewProjMat;

		std::unordered_map<std::string, VertexArray*> MeshVertexArrays;
		std::unordered_map<std::string, Texture2D*> MaterialTextures;
		Shader* CurrentShader;




		std::vector<glm::vec3>		UnitCubePositions;
		std::vector<glm::vec3>		UnitCubeNormals;
		std::vector<IndexType>		UnitCubeIndices;

		Batch<ColorVertex3D, IndexType>* ColorBatch;
		Shader* UnlitColorShader;
		Shader* LitColorShader;

	};

	static Renderer3DData s_Data3D;



	void Renderer3D::Init()
	{
		s_Data3D.CurrentShader = new Shader;
		s_Data3D.CurrentShader->AddVertexShader("res/shaders/Blinn-Phong.vert");
		s_Data3D.CurrentShader->AddFragmentShader("res/shaders/Blinn-Phong.frag");
		s_Data3D.CurrentShader->Link();
		s_Data3D.CurrentShader->Bind();


		//s_Data3D.UnitCubePositions.resize(24);
		//s_Data3D.UnitCubePositions = {
		//	{ 0, 0, 0 }, // 1
		//	{ 1, 0, 0 },
		//	{ 1, 1, 0 },
		//	{ 0, 1, 0 },
		//	{ 0, 0, 1 }, // 2
		//	{ 1, 0, 1 },
		//	{ 1, 1, 1 },
		//	{ 0, 1, 1 },
		//	{ 0, 0, 1 }, // 3
		//	{ 1, 0, 1 },
		//	{ 1, 0, 0 },
		//	{ 0, 0, 0 },
		//	{ 0, 1, 1 }, // 4
		//	{ 1, 1, 1 },
		//	{ 1, 1, 0 },
		//	{ 0, 1, 0 },
		//	{ 0, 0, 1 }, // 5
		//	{ 0, 0, 0 },
		//	{ 0, 1, 0 },
		//	{ 0, 1, 1 },
		//	{ 1, 0, 1 }, // 6
		//	{ 1, 0, 0 },
		//	{ 1, 1, 0 },
		//	{ 1, 1, 1 }
		//};

		//s_Data3D.UnitCubeNormals.resize(6);
		//s_Data3D.UnitCubeNormals = {
		//	{ 0, 0, -1 }, // First 4 vertices
		//	{ 0, 0, 1 },
		//	{ 0, -1, 0 },
		//	{ 0, 1, 0 },
		//	{ -1, 0, 0 },
		//	{ 1, 0, 0 } // Last 4 vertices
		//};

		//s_Data3D.UnitCubeIndices.resize(36);
		//s_Data3D.UnitCubeIndices = {
		//	0, 1, 2, 0, 2, 3,
		//	4, 5, 6, 4, 6, 7,
		//	8, 9, 10, 8, 10, 11,
		//	12, 13, 14, 12, 14, 15,
		//	16, 17, 18, 16, 18, 19,
		//	20, 21, 22, 20, 22, 23
		//};

		//s_Data3D.ColorBatch = new Batch<ColorVertex3D, IndexType>(10000, 60000);
		//s_Data3D.ColorBatch->VBO = new VertexBuffer(s_Data3D.ColorBatch->VBOSize);
		//s_Data3D.ColorBatch->VBO->AddLayout(0, DataType::Float32, 3); // Position
		//s_Data3D.ColorBatch->VBO->AddLayout(1, DataType::Float32, 4); // Color
		//s_Data3D.ColorBatch->VBO->AddLayout(2, DataType::Float32, 3); // Normal
		//s_Data3D.ColorBatch->IBO = new IndexBuffer(s_Data3D.ColorBatch->IBOSize);
		//s_Data3D.ColorBatch->VAO = new VertexArray(*s_Data3D.ColorBatch->VBO, *s_Data3D.ColorBatch->IBO);

		//s_Data3D.LitColorShader = new Shader();
		//s_Data3D.LitColorShader->AddVertexShader("res/shaders/Color3DLighting.vert");
		//s_Data3D.LitColorShader->AddFragmentShader("res/shaders/Color3DLighting.frag");
		//s_Data3D.LitColorShader->Link();
		//s_Data3D.LitColorShader->Bind();

		//// Material struct
		//s_Data3D.LitColorShader->SetVec3("u_Material.Color", { 0.5f, 0.3f, 0.1f });
		//s_Data3D.LitColorShader->SetVec3("u_Material.Specular", { 0.5f, 0.3f, 0.1f });
		//s_Data3D.LitColorShader->SetFloat("u_Material.Shininess", 32.0f);

		//s_Data3D.UnlitColorShader = new Shader();
		//s_Data3D.UnlitColorShader->AddVertexShader("res/shaders/FlatColor3D.vert");
		//s_Data3D.UnlitColorShader->AddFragmentShader("res/shaders/FlatColor3D.frag");
		//s_Data3D.UnlitColorShader->Link();
		//s_Data3D.UnlitColorShader->Bind();

	} 

	void Renderer3D::BeginScene(const Camera& camera)
	{
		s_Data3D.SceneViewProjMat = camera.GetViewProjMat();

		s_Data3D.CurrentShader->Bind();
		s_Data3D.CurrentShader->SetMat4("u_ViewProjMatrix", s_Data3D.SceneViewProjMat);
	}

	void Renderer3D::BeginScene(const Camera& camera, const Lighting& lights)
	{
		BeginScene(camera);

		s_Data3D.CurrentShader->SetVec3("u_ViewPos", camera.GetPos());

		// Setup point lights.
		for (uint32_t i = 0; i < lights.PointLights.size(); i++)
		{
			const PointLight& l = lights.PointLights[i];
			char buff[100];

			snprintf(buff, 100, "u_PointLights[%i].Position", i);
			s_Data3D.CurrentShader->SetVec3(buff, l.Position);

			snprintf(buff, 100, "u_PointLights[%i].Ambient", i);
			s_Data3D.CurrentShader->SetVec3(buff, l.Ambient);
			snprintf(buff, 100, "u_PointLights[%i].Diffuse", i);
			s_Data3D.CurrentShader->SetVec3(buff, l.Diffuse);
			snprintf(buff, 100, "u_PointLights[%i].Specular", i);
			s_Data3D.CurrentShader->SetVec3(buff, l.Specular);

			snprintf(buff, 100, "u_PointLights[%i].Constant", i);
			s_Data3D.CurrentShader->SetFloat(buff, l.Constant);
			snprintf(buff, 100, "u_PointLights[%i].Linear", i);
			s_Data3D.CurrentShader->SetFloat(buff, l.Linear);
			snprintf(buff, 100, "u_PointLights[%i].Quadratic", i);
			s_Data3D.CurrentShader->SetFloat(buff, l.Quadratic);
		}

	}

	void Renderer3D::EndScene()
	{
		//s_Data3D.ColorBatch->FlushAndReset();
	}


	static VertexArray* GenerateGLBuffers(const Mesh& mesh)
	{
		VertexBuffer* vbo = new VertexBuffer(mesh.Vertices.data(), sizeof(Vertex) * mesh.Vertices.size());
		IndexBuffer* ibo = new IndexBuffer(mesh.Indices.data(), sizeof(uint32_t) * mesh.Indices.size());

		vbo->AddLayout(0, DataType::Float32, 3); // Position
		vbo->AddLayout(1, DataType::Float32, 2); // TexCoord
		vbo->AddLayout(2, DataType::Float32, 3); // Normal

		VertexArray* vao = new VertexArray(vbo, ibo);

		LOG_DEBUG("Create GL buffers: VBO {} w/ {} vertices IBO {} w/ {} indices", vbo->GetId(), mesh.Vertices.size(), ibo->GetId(), mesh.Indices.size());

		return vao;
	}

	static Texture2D* GenerateGLTexture(const std::string& path)
	{
		Texture2D* tex = new Texture2D();
		tex->SetImage(path);

		return tex;
	}

	static VertexArray* GetMeshVAO(const Mesh& mesh)
	{
		if (s_Data3D.MeshVertexArrays.find(mesh.Name) == s_Data3D.MeshVertexArrays.end())
		{
			// Mesh has not had buffers generated yet. Generate them.
			s_Data3D.MeshVertexArrays[mesh.Name] = GenerateGLBuffers(mesh);
		}

		return s_Data3D.MeshVertexArrays[mesh.Name];
	}

	static Texture2D* GetMaterialTexture(const std::string& texPath)
	{
		if (s_Data3D.MaterialTextures.find(texPath) == s_Data3D.MaterialTextures.end())
		{
			// Texture has not been generated yet.
			s_Data3D.MaterialTextures[texPath] = GenerateGLTexture(texPath);
		}

		return s_Data3D.MaterialTextures[texPath];
	}



	void Renderer3D::DrawScene(const Scene& scene, const glm::mat4& transform)
	{
		DrawNode(*scene.RootNode, transform);
	}

	void Renderer3D::DrawNode(const Node& node, const glm::mat4& parentTransform)
	{
		glm::mat4 transform = node.Transform * parentTransform;

		// Draw meshes for this node.
		for (auto& mesh : node.Meshes)
		{
			DrawMesh(*mesh, transform);
		}

		// Draw node's children recursively.
		for (auto& child : node.Children)
		{
			DrawNode(*child, transform);
		}
	}

	void Renderer3D::DrawMesh(const Mesh& mesh, const glm::mat4& transform)
	{
		VertexArray* vao = GetMeshVAO(mesh);
		Texture2D* tex = GetMaterialTexture(mesh.Material->DiffuseMapPath);
		tex->Bind(0);

		// Set material properties.
		Material& mat = *mesh.Material;
		//s_Data3D.CurrentShader->SetVec3("u_Material.AmbientColor", mat.AmbientColor);
		//s_Data3D.CurrentShader->SetVec3("u_Material.DiffuseColor", mat.DiffuseColor);
		s_Data3D.CurrentShader->SetVec3("u_Material.SpecularColor", mat.SpecularColor);
		s_Data3D.CurrentShader->SetFloat("u_Material.Shininess", mat.Shininess);

		s_Data3D.CurrentShader->SetInt("u_Material.DiffuseMap", 0);

		// Set transform.
		s_Data3D.CurrentShader->SetMat4("u_Transform", transform);

		vao->Draw(DrawMode::Triangles, mesh.Indices.size());
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
			indices[i] = s_Data3D.ColorBatch->VertexCount + s_Data3D.UnitCubeIndices[i];
		}

		s_Data3D.ColorBatch->AddData(vertices, sizeof(vertices));
		s_Data3D.ColorBatch->AddIndices(indices, sizeof(indices));
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

		for (uint32_t j = 0; j < (nStacks + 1); j++)
		{
			for (uint32_t i = 0; i < nSegments; i++)
			{
				glm::vec3 norm = {
					glm::sin(phi)* glm::cos(theta),
					glm::sin(phi)* glm::sin(theta),
					glm::cos(phi)
				};

				glm::vec3 pos = centre + (radius * norm);

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
				uint32_t i0 = s_Data3D.ColorBatch->VertexCount + i + (nSegments * j);
				uint32_t i1 = i0 + nSegments;
				uint32_t i3 = s_Data3D.ColorBatch->VertexCount + ((i + 1) % nSegments) + (nSegments * j);
				uint32_t i2 = i3 + nSegments;

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

		s_Data3D.ColorBatch->AddData(vertices.data(), vertices.size() * sizeof(ColorVertex3D));
		s_Data3D.ColorBatch->AddIndices(indices.data(), indices.size() * sizeof(IndexType));
	}

}