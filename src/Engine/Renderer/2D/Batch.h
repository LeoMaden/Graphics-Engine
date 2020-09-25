#pragma once
#include <cstdint>

#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"

typedef unsigned char byte;

namespace Engine {

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

			BatchDataInsPtr = BatchData;
			BatchIndicesInsPtr = BatchIndices;
		}

		~Batch()
		{
			delete[] BatchData;
			delete[] BatchIndices;
		}

		Shader* Shader = nullptr;
		DrawMode DrawMode = DrawMode::Triangles;
		DataType IBOType = DataType::UInt16;

		VertexBuffer* VBO = nullptr;
		IndexBuffer* IBO = nullptr;
		VertexArray* VAO = nullptr;

		uint32_t VertexCount = 0;
		uint32_t MaxVertexCount;
		uint32_t VBOSize;
		byte* BatchData = nullptr;
		byte* BatchDataInsPtr = nullptr;

		uint32_t IndexCount = 0;
		uint32_t MaxIndexCount;
		uint32_t IBOSize;
		byte* BatchIndices = nullptr;
		byte* BatchIndicesInsPtr = nullptr;

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

			//VBO->Bind();
			VBO->SetData(BatchData, VertexCount * sizeof(VBO_T));

			//IBO->Bind();
			IBO->SetIndices(BatchIndices, IndexCount * sizeof(IBO_T));

			RenderCommand::DrawIndexed(DrawMode, IndexCount, IBOType);
			// TODO: Stats for both renderers Renderer2D::Stats.Draws++;
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

}