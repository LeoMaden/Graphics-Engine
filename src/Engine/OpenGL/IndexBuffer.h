#pragma once
#include <vector>
#include <glad/glad.h>

#include "BufferType.h"

namespace Engine {

	class IndexBuffer
	{
	public:
		// ----- Static IndexBuffers -----

		// Create an IndexBuffer with fixed indices.
		// Size in bytes.
		IndexBuffer(const void* indices, uint32_t size);


		// ----- Dynamic IndexBuffers -----

		// Create an IndexBuffer with a given capacity (in bytes)
		// which can have indices written to it at will.
		IndexBuffer(uint32_t capacity);

		// Set the indices in a dynamic IndexBuffer.
		// Size is given in bytes.
		void SetIndices(const void* indices, uint32_t size);


		// ----- All IndexBuffers -----
		~IndexBuffer();


		uint32_t		GetId()		const { return m_Id; }
		BufferType		GetType()	const { return m_Type; }

	private:
		GLuint			m_Id;
		uint32_t		m_Capacity;
		BufferType		m_Type;
	};

} 