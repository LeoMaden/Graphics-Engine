#include "IndexBuffer.h"

#include "Log.h"

namespace Engine {

	IndexBuffer::IndexBuffer(const void* indices, uint32_t size)
		: m_Id(0), m_Capacity(size), m_Type(BufferType::Static)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}


	IndexBuffer::IndexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity), m_Type(BufferType::Dynamic)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::SetIndices(const void* indices, uint32_t size)
	{
		ASSERT(size <= m_Capacity, "Buffer is not large enough");
		ASSERT(m_Type == BufferType::Dynamic, "Buffer must be dynamic");

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
	}


	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

}