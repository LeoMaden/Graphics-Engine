#include "IndexBuffer.h"

#include "Log.h"

namespace Engine {

	IndexBuffer::IndexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity, nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void IndexBuffer::SetIndices(void* indices, uint32_t size)
	{
		ASSERT(size < m_Capacity, "Buffer is not large enough");

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
	}

}