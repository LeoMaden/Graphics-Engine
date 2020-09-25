#include "Pch.h"
#include "Renderer2D.h"
#include "Log.h"
#include "RenderUtils.h"
#include "RenderCommand.h"
#include "Batch.h"

#include <glad/glad.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Engine {

	struct ColorVertex2D
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct TextureVertex2D
	{
		glm::vec3	Position;
		glm::vec2	TexCoord;
		float		TexSlot;
		//float		TilingFactor;
	};

	using IndexType = uint16_t;

	struct Renderer2DData
	{
		glm::vec2		UnitSquarePositions[4];
		glm::mat4		SceneViewProjMat;

		uint32_t MaxQuadsPerBatch = 10000;

		Batch<ColorVertex2D, IndexType>*		FlatColBatch;
		Batch<TextureVertex2D, IndexType>*	TextureBatch;
		Batch<ColorVertex2D, IndexType>*		LineBatch;
		Batch<ColorVertex2D, IndexType>*		PointBatch;

		int CurrentTexSlot = 0;
		int BoundTextureIds[32];
	};

	static Renderer2DData s_Data2D;
	Renderer2D::Statistics Renderer2D::Stats;

	static void ResetTextures()
	{
		for (int i = 0; i < 32; i++)
		{
			s_Data2D.BoundTextureIds[i] = 0;
		}
		s_Data2D.CurrentTexSlot = 0;
	}

	/*static*/ void Renderer2D::Init()
	{
		s_Data2D.UnitSquarePositions[0] = { 0.0f, 0.0f };
		s_Data2D.UnitSquarePositions[1] = { 1.0f, 0.0f };
		s_Data2D.UnitSquarePositions[2] = { 1.0f, 1.0f };
		s_Data2D.UnitSquarePositions[3] = { 0.0f, 1.0f };

		uint32_t& q = s_Data2D.MaxQuadsPerBatch;

		s_Data2D.FlatColBatch = new Batch<ColorVertex2D, IndexType>(4 * q, 6 * q);
		s_Data2D.FlatColBatch->VBO = new VertexBuffer(s_Data2D.FlatColBatch->VBOSize);
		s_Data2D.FlatColBatch->VBO->AddLayout(0, DataType::Float32, 3); // Position
		s_Data2D.FlatColBatch->VBO->AddLayout(1, DataType::Float32, 4); // Color
		s_Data2D.FlatColBatch->IBO = new IndexBuffer(s_Data2D.FlatColBatch->IBOSize);
		s_Data2D.FlatColBatch->VAO = new VertexArray(s_Data2D.FlatColBatch->VBO, s_Data2D.FlatColBatch->IBO);

		s_Data2D.TextureBatch = new Batch<TextureVertex2D, IndexType>(4 * q, 6 * q);
		s_Data2D.TextureBatch->VBO = new VertexBuffer(s_Data2D.TextureBatch->VBOSize);
		s_Data2D.TextureBatch->VBO->AddLayout(0, DataType::Float32, 3); // Position
		s_Data2D.TextureBatch->VBO->AddLayout(1, DataType::Float32, 2); // Tex coord
		s_Data2D.TextureBatch->VBO->AddLayout(2, DataType::Float32, 1); // Tex id
		s_Data2D.TextureBatch->IBO = new IndexBuffer(s_Data2D.TextureBatch->IBOSize);
		s_Data2D.TextureBatch->VAO = new VertexArray(s_Data2D.TextureBatch->VBO, s_Data2D.TextureBatch->IBO);

		s_Data2D.LineBatch = new Batch<ColorVertex2D, IndexType>(100, 100);
		s_Data2D.LineBatch->DrawMode = DrawMode::Lines;
		s_Data2D.LineBatch->VBO = new VertexBuffer(s_Data2D.LineBatch->VBOSize);
		s_Data2D.LineBatch->VBO->AddLayout(0, DataType::Float32, 3); // Position
		s_Data2D.LineBatch->VBO->AddLayout(1, DataType::Float32, 4); // Color
		s_Data2D.LineBatch->IBO = new IndexBuffer(s_Data2D.LineBatch->IBOSize);
		s_Data2D.LineBatch->VAO = new VertexArray(s_Data2D.LineBatch->VBO, s_Data2D.LineBatch->IBO);

		s_Data2D.PointBatch = new Batch<ColorVertex2D, IndexType>(100, 100);
		s_Data2D.PointBatch->DrawMode = DrawMode::Points;
		s_Data2D.PointBatch->VBO = new VertexBuffer(s_Data2D.PointBatch->VBOSize);
		s_Data2D.PointBatch->VBO->AddLayout(0, DataType::Float32, 3); // Position
		s_Data2D.PointBatch->VBO->AddLayout(1, DataType::Float32, 4); // Color
		s_Data2D.PointBatch->IBO = new IndexBuffer(s_Data2D.PointBatch->IBOSize);
		s_Data2D.PointBatch->VAO = new VertexArray(s_Data2D.PointBatch->VBO, s_Data2D.PointBatch->IBO);

		s_Data2D.FlatColBatch->Shader = new Shader();
		s_Data2D.FlatColBatch->Shader->AddVertexShader("res/shaders/FlatColor.vert");
		s_Data2D.FlatColBatch->Shader->AddFragmentShader("res/shaders/FlatColor.frag");
		s_Data2D.FlatColBatch->Shader->Link();

		s_Data2D.TextureBatch->Shader = new Shader();
		s_Data2D.TextureBatch->Shader->AddVertexShader("res/shaders/Texture2D.vert");
		s_Data2D.TextureBatch->Shader->AddFragmentShader("res/shaders/Texture2D.frag");
		s_Data2D.TextureBatch->Shader->Link();

		s_Data2D.LineBatch->Shader = s_Data2D.FlatColBatch->Shader;

		s_Data2D.PointBatch->Shader = s_Data2D.FlatColBatch->Shader;

		s_Data2D.TextureBatch->Shader->Bind();
		int texIds[32];
		for (int i = 0; i < 32; i++)
		{
			texIds[i] = i;
		}
		s_Data2D.TextureBatch->Shader->SetIntArray("u_Textures", texIds, 32);

	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		s_Data2D.FlatColBatch->Shader->SetMat4("u_Transform", s_Data2D.SceneViewProjMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data2D.SceneViewProjMat = camera.GetViewProjMat();

		s_Data2D.FlatColBatch->Shader->Bind();
		s_Data2D.FlatColBatch->Shader->SetMat4("u_Transform", s_Data2D.SceneViewProjMat);

		s_Data2D.TextureBatch->Shader->Bind();
		s_Data2D.TextureBatch->Shader->SetMat4("u_Transform", s_Data2D.SceneViewProjMat);

		s_Data2D.FlatColBatch->Reset();
		s_Data2D.TextureBatch->Reset();
		s_Data2D.LineBatch->Reset();
		s_Data2D.PointBatch->Reset();
		ResetTextures();
	}

	void Renderer2D::EndScene()
	{
		s_Data2D.FlatColBatch->FlushAndReset();
		s_Data2D.TextureBatch->FlushAndReset();
		s_Data2D.LineBatch->FlushAndReset();
		s_Data2D.PointBatch->FlushAndReset();
		ResetTextures();
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, color, size / 2.0f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& centre)
	{
		if (s_Data2D.FlatColBatch->EnoughSpace(4, 6) == false)
		{
			s_Data2D.FlatColBatch->FlushAndReset();
		}

		ColorVertex2D vertices[4];
		IndexType indices[6];

		indices[0] = 0 + s_Data2D.FlatColBatch->VertexCount;
		indices[1] = 1 + s_Data2D.FlatColBatch->VertexCount;
		indices[2] = 2 + s_Data2D.FlatColBatch->VertexCount;
		indices[3] = 2 + s_Data2D.FlatColBatch->VertexCount;
		indices[4] = 3 + s_Data2D.FlatColBatch->VertexCount;
		indices[5] = 0 + s_Data2D.FlatColBatch->VertexCount;

		vertices[0].Position = glm::vec3(s_Data2D.UnitSquarePositions[0] * size - centre + position, 0.0f);
		vertices[1].Position = glm::vec3(s_Data2D.UnitSquarePositions[1] * size - centre + position, 0.0f);
		vertices[2].Position = glm::vec3(s_Data2D.UnitSquarePositions[2] * size - centre + position, 0.0f);
		vertices[3].Position = glm::vec3(s_Data2D.UnitSquarePositions[3] * size - centre + position, 0.0f);

		vertices[0].Color = color;
		vertices[1].Color = color;
		vertices[2].Color = color;
		vertices[3].Color = color;

		s_Data2D.FlatColBatch->AddData(vertices, sizeof(vertices));
		s_Data2D.FlatColBatch->AddIndices(indices, sizeof(indices));

		Stats.Quads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture)
	{
		DrawQuad(position, size, texture, size / 2.0f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture, const glm::vec2& centre)
	{
		if (s_Data2D.TextureBatch->EnoughSpace(4, 6) == false)
		{
			s_Data2D.TextureBatch->FlushAndReset();
			ResetTextures();
		}

		int texId = -1;
		for (int i = 0; i < s_Data2D.CurrentTexSlot; i++)
		{
			if (s_Data2D.BoundTextureIds[i] == texture.GetId())
			{
				texId = i;
				break;
			}
		}
		if (texId == -1)
		{
			texId = s_Data2D.CurrentTexSlot;
			s_Data2D.BoundTextureIds[texId] = texture.GetId();

			texture.Bind(texId);

			s_Data2D.CurrentTexSlot++;
			Stats.Textures = s_Data2D.CurrentTexSlot;
		}

		TextureVertex2D vertices[4];
		IndexType indices[6];

		indices[0] = 0 + s_Data2D.TextureBatch->VertexCount;
		indices[1] = 1 + s_Data2D.TextureBatch->VertexCount;
		indices[2] = 2 + s_Data2D.TextureBatch->VertexCount;
		indices[3] = 2 + s_Data2D.TextureBatch->VertexCount;
		indices[4] = 3 + s_Data2D.TextureBatch->VertexCount;
		indices[5] = 0 + s_Data2D.TextureBatch->VertexCount;

		vertices[0].Position = glm::vec3((s_Data2D.UnitSquarePositions[0] - centre) * size + position, 0.0f);
		vertices[1].Position = glm::vec3((s_Data2D.UnitSquarePositions[1] - centre) * size + position, 0.0f);
		vertices[2].Position = glm::vec3((s_Data2D.UnitSquarePositions[2] - centre) * size + position, 0.0f);
		vertices[3].Position = glm::vec3((s_Data2D.UnitSquarePositions[3] - centre) * size + position, 0.0f);

		vertices[0].TexCoord = texture.GetTexCoord(0);
		vertices[1].TexCoord = texture.GetTexCoord(1);
		vertices[2].TexCoord = texture.GetTexCoord(2);
		vertices[3].TexCoord = texture.GetTexCoord(3);

		vertices[0].TexSlot = texId;
		vertices[1].TexSlot = texId;
		vertices[2].TexSlot = texId;
		vertices[3].TexSlot = texId;

		s_Data2D.TextureBatch->AddData(vertices, sizeof(vertices));
		s_Data2D.TextureBatch->AddIndices(indices, sizeof(indices));

		Stats.Quads++;

	}

	void Renderer2D::DrawCircle(const glm::vec2& centre, float radius, const glm::vec4& color, uint32_t nDivisions)
	{
		std::vector<ColorVertex2D> vertices;
		std::vector<IndexType> indices;

		uint32_t nVert = nDivisions;
		uint32_t nTri = nDivisions - 2;
		uint32_t nInd = 3 * nTri;

		if (s_Data2D.FlatColBatch->EnoughSpace(nVert, nInd) == false)
		{
			s_Data2D.FlatColBatch->FlushAndReset();
		}

		vertices.resize(nVert);
		indices.resize(nInd);

		// Vertices
		double theta = 0.0f;
		double dTheta = glm::two_pi<double>() / (double)nDivisions;

		for (int i = 0; i < nVert; i++)
		{
			vertices[i].Position = { 
				centre.x + radius * glm::sin(theta), 
				centre.y + radius * glm::cos(theta), 
				0.0f };

			vertices[i].Color = color;

			theta += dTheta;
		}

		// Indices
		for (uint64_t i = 0; i < nTri; i++)
		{
			uint64_t j = 3 * i;

			indices[j]		= 0 + s_Data2D.FlatColBatch->VertexCount;
			indices[j + 1]	= i + 1 + s_Data2D.FlatColBatch->VertexCount;
			indices[j + 2]	= i + 2 + s_Data2D.FlatColBatch->VertexCount;
		}

		s_Data2D.FlatColBatch->AddData(vertices.data(), vertices.size() * sizeof(ColorVertex2D));
		s_Data2D.FlatColBatch->AddIndices(indices.data(), indices.size() * sizeof(IndexType));

		Stats.Circles++;
	}

	void Renderer2D::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
	{
		if (s_Data2D.LineBatch->EnoughSpace(2, 2) == false)
		{
			s_Data2D.LineBatch->FlushAndReset();
		}

		ColorVertex2D vertices[2];
		vertices[0].Position = glm::vec3(start, 0.0f);
		vertices[1].Position = glm::vec3(end, 0.0f);

		vertices[0].Color = color;
		vertices[1].Color = color;

		IndexType indices[2];
		indices[0] = s_Data2D.LineBatch->VertexCount;
		indices[1] = s_Data2D.LineBatch->VertexCount + 1;

		s_Data2D.LineBatch->AddData(vertices, sizeof(vertices));
		s_Data2D.LineBatch->AddIndices(indices, sizeof(indices));

		Stats.Lines++;
	}

	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec4& color)
	{
		ColorVertex2D vertex;
		vertex.Position = glm::vec3(position, 0.0f);
		vertex.Color = color;

		IndexType index = s_Data2D.PointBatch->VertexCount;

		s_Data2D.PointBatch->AddData(&vertex, sizeof(ColorVertex2D));
		s_Data2D.PointBatch->AddIndices(&index, sizeof(IndexType));

		Stats.Points++;
	}


}