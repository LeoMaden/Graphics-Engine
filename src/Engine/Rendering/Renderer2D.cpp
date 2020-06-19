#include "Renderer2D.h"
#include "Log.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	struct ColorVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct TextureVertex
	{
		glm::vec3	Position;
		glm::vec2	TexCoord;
		float		TexSlot;
		//float		TilingFactor;
	};

	template<typename VBO_T, typename IBO_T>
	struct Batch
	{
		Batch(uint32_t maxVertexCount, uint32_t maxIndexCount)
			: MaxVertexCount(maxVertexCount), MaxIndexCount(maxIndexCount)
		{
			VBOSize = MaxVertexCount * sizeof(VBO_T);
			IBOSize = MaxIndexCount * sizeof(IBO_T);

			BatchData = new byte[VBOSize];
			BatchIndices = new byte[IBOSize];
		}

		~Batch()
		{
			delete[] BatchData;
			delete[] BatchIndices;
		}

		Shader* Shader = nullptr;

		VertexBuffer* VBO	= nullptr;
		IndexBuffer* IBO	= nullptr;
		VertexArray* VAO	= nullptr;

		uint32_t VertexCount	= 0;
		uint32_t MaxVertexCount;
		uint32_t VBOSize;
		byte* BatchData;
		byte* BatchDataInsPtr;

		uint32_t IndexCount			= 0;
		uint32_t MaxIndexCount;
		uint32_t IBOSize;
		byte* BatchIndices;
		byte* BatchIndicesInsPtr;

		void AddData(VBO_T* data, uint32_t size)
		{
			VertexCount += size / sizeof(VBO_T);

			memcpy(BatchDataInsPtr, data, size);
			BatchDataInsPtr += size;
		}

		void AddIndices(IBO_T* indices, uint32_t size)
		{
			IndexCount += size / sizeof(IBO_T);

			memcpy(BatchIndicesInsPtr, indices, size);
			BatchIndicesInsPtr += size;
		}

		bool EnoughSpace(uint32_t numVertex, uint32_t numIndex)
		{
			bool v = VertexCount + numVertex <= MaxVertexCount;
			bool i = IndexCount + numIndex <= MaxIndexCount;

			return v && i;
		}

		void Flush()
		{
			if (VertexCount == 0) return;

			Shader->Bind();

			VAO->Bind();

			VBO->Bind();
			VBO->SetData(BatchData, VertexCount * sizeof(VBO_T));

			IBO->Bind();
			IBO->SetIndices(BatchIndices, IndexCount * sizeof(IBO_T));

			glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);
			Renderer2D::Stats.Draws++;
		}

		void Reset()
		{
			VertexCount = 0;
			IndexCount = 0;

			BatchDataInsPtr = BatchData;
			BatchIndicesInsPtr = BatchIndices;
		}

		void FlushAndReset()
		{
			Flush();
			Reset();
		}
	};

	using IndexType = uint16_t;

	struct Renderer2DData
	{
		glm::vec2		UnitSquarePositions[4];
		glm::mat4		SceneViewProjMat;

		uint32_t MaxQuadsPerBatch = 10000;

		Batch<ColorVertex, IndexType>*			FlatColBatch;
		Batch<TextureVertex, IndexType>*	TextureBatch;

		int CurrentTexSlot = 0;
		int BoundTextureIds[32];
	};

	static Renderer2DData s_Data;
	Renderer2D::Statistics Renderer2D::Stats;

	static void ResetTextures()
	{
		for (int i = 0; i < 32; i++)
		{
			s_Data.BoundTextureIds[i] = 0;
		}
		s_Data.CurrentTexSlot = 0;
	}

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

		s_Data.UnitSquarePositions[0] = { 0.0f, 0.0f };
		s_Data.UnitSquarePositions[1] = { 1.0f, 0.0f };
		s_Data.UnitSquarePositions[2] = { 1.0f, 1.0f };
		s_Data.UnitSquarePositions[3] = { 0.0f, 1.0f };

		uint32_t& q = s_Data.MaxQuadsPerBatch;

		s_Data.FlatColBatch = new Batch<ColorVertex, IndexType>(4 * q, 6 * q);
		s_Data.FlatColBatch->VBO = new VertexBuffer(s_Data.FlatColBatch->VBOSize);
		s_Data.FlatColBatch->VBO->AddLayout(0, GL_FLOAT, 3); // Position
		s_Data.FlatColBatch->VBO->AddLayout(1, GL_FLOAT, 4); // Color
		s_Data.FlatColBatch->IBO = new IndexBuffer(s_Data.FlatColBatch->IBOSize);
		s_Data.FlatColBatch->VAO = new VertexArray(*s_Data.FlatColBatch->VBO, *s_Data.FlatColBatch->IBO);

		s_Data.TextureBatch = new Batch<TextureVertex, IndexType>(4 * q, 6 * q);
		s_Data.TextureBatch->VBO = new VertexBuffer(s_Data.TextureBatch->VBOSize);
		s_Data.TextureBatch->VBO->AddLayout(0, GL_FLOAT, 3); // Position
		s_Data.TextureBatch->VBO->AddLayout(1, GL_FLOAT, 2); // Tex coord
		s_Data.TextureBatch->VBO->AddLayout(2, GL_FLOAT, 1); // Tex id
		s_Data.TextureBatch->IBO = new IndexBuffer(s_Data.TextureBatch->IBOSize);
		s_Data.TextureBatch->VAO = new VertexArray(*s_Data.TextureBatch->VBO, *s_Data.TextureBatch->IBO);

		s_Data.FlatColBatch->Shader = new Shader();
		s_Data.FlatColBatch->Shader->AddVertexShader("res/shaders/FlatColor.vert");
		s_Data.FlatColBatch->Shader->AddFragmentShader("res/shaders/FlatColor.frag");
		s_Data.FlatColBatch->Shader->Link();

		s_Data.TextureBatch->Shader = new Shader();
		s_Data.TextureBatch->Shader->AddVertexShader("res/shaders/Texture2D.vert");
		s_Data.TextureBatch->Shader->AddFragmentShader("res/shaders/Texture2D.frag");
		s_Data.TextureBatch->Shader->Link();
		s_Data.TextureBatch->Shader->Bind();

		int texIds[32];
		for (int i = 0; i < 32; i++)
		{
			texIds[i] = i;
		}
		s_Data.TextureBatch->Shader->SetIntArray("u_Textures", texIds, 32);
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		s_Data.FlatColBatch->Shader->SetMat4("u_Transform", s_Data.SceneViewProjMat);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.SceneViewProjMat = camera.GetViewProjMat();

		s_Data.FlatColBatch->Shader->Bind();
		s_Data.FlatColBatch->Shader->SetMat4("u_Transform", s_Data.SceneViewProjMat);

		s_Data.TextureBatch->Shader->Bind();
		s_Data.TextureBatch->Shader->SetMat4("u_Transform", s_Data.SceneViewProjMat);

		s_Data.FlatColBatch->Reset();
		s_Data.TextureBatch->Reset();
		ResetTextures();
	}

	void Renderer2D::EndScene()
	{
		s_Data.FlatColBatch->FlushAndReset();
		s_Data.TextureBatch->FlushAndReset();
		ResetTextures();
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, color, size / 2.0f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& centre)
	{
		if (s_Data.FlatColBatch->EnoughSpace(4, 6) == false)
		{
			s_Data.FlatColBatch->FlushAndReset();
		}

		ColorVertex vertices[4];
		IndexType indices[6];

		indices[0] = 0 + s_Data.FlatColBatch->VertexCount;
		indices[1] = 1 + s_Data.FlatColBatch->VertexCount;
		indices[2] = 2 + s_Data.FlatColBatch->VertexCount;
		indices[3] = 2 + s_Data.FlatColBatch->VertexCount;
		indices[4] = 3 + s_Data.FlatColBatch->VertexCount;
		indices[5] = 0 + s_Data.FlatColBatch->VertexCount;

		vertices[0].Position = glm::vec3((s_Data.UnitSquarePositions[0] - centre) * size + position, 0.0f);
		vertices[1].Position = glm::vec3((s_Data.UnitSquarePositions[1] - centre) * size + position, 0.0f);
		vertices[2].Position = glm::vec3((s_Data.UnitSquarePositions[2] - centre) * size + position, 0.0f);
		vertices[3].Position = glm::vec3((s_Data.UnitSquarePositions[3] - centre) * size + position, 0.0f);

		vertices[0].Color = color;
		vertices[1].Color = color;
		vertices[2].Color = color;
		vertices[3].Color = color;

		s_Data.FlatColBatch->AddData(vertices, sizeof(vertices));
		s_Data.FlatColBatch->AddIndices(indices, sizeof(indices));

		Stats.Quads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture)
	{
		DrawQuad(position, size, texture, size / 2.0f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2DBase& texture, const glm::vec2& centre)
	{
		if (s_Data.TextureBatch->EnoughSpace(4, 6) == false)
		{
			s_Data.TextureBatch->FlushAndReset();
			ResetTextures();
		}

		int texId = -1;
		for (int i = 0; i < s_Data.CurrentTexSlot; i++)
		{
			if (s_Data.BoundTextureIds[i] == texture.GetId())
			{
				texId = i;
				break;
			}
		}
		if (texId == -1)
		{
			texId = s_Data.CurrentTexSlot;
			s_Data.BoundTextureIds[texId] = texture.GetId();

			texture.Bind(texId);

			s_Data.CurrentTexSlot++;
			Stats.Textures = s_Data.CurrentTexSlot;
		}

		TextureVertex vertices[4];
		IndexType indices[6];

		indices[0] = 0 + s_Data.TextureBatch->VertexCount;
		indices[1] = 1 + s_Data.TextureBatch->VertexCount;
		indices[2] = 2 + s_Data.TextureBatch->VertexCount;
		indices[3] = 2 + s_Data.TextureBatch->VertexCount;
		indices[4] = 3 + s_Data.TextureBatch->VertexCount;
		indices[5] = 0 + s_Data.TextureBatch->VertexCount;

		vertices[0].Position = glm::vec3((s_Data.UnitSquarePositions[0] - centre) * size + position, 0.0f);
		vertices[1].Position = glm::vec3((s_Data.UnitSquarePositions[1] - centre) * size + position, 0.0f);
		vertices[2].Position = glm::vec3((s_Data.UnitSquarePositions[2] - centre) * size + position, 0.0f);
		vertices[3].Position = glm::vec3((s_Data.UnitSquarePositions[3] - centre) * size + position, 0.0f);

		vertices[0].TexCoord = texture.GetTexCoord(0);
		vertices[1].TexCoord = texture.GetTexCoord(1);
		vertices[2].TexCoord = texture.GetTexCoord(2);
		vertices[3].TexCoord = texture.GetTexCoord(3);

		vertices[0].TexSlot = texId;
		vertices[1].TexSlot = texId;
		vertices[2].TexSlot = texId;
		vertices[3].TexSlot = texId;

		s_Data.TextureBatch->AddData(vertices, sizeof(vertices));
		s_Data.TextureBatch->AddIndices(indices, sizeof(indices));

		Stats.Quads++;

	}

}