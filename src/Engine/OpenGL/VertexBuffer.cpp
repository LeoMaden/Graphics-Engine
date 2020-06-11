#include "VertexBuffer.h"

#include "Log.h"

namespace Engine {

	VertexBuffer::VertexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity), m_Layout()
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void VertexBuffer::AddLayout(uint32_t location, GLenum type, uint32_t count)
	{
		m_Layout.push_back({ location, type, count });
	}

	void VertexBuffer::SetData(void* data, uint32_t size)
	{
		ASSERT(size < m_Capacity, "Buffer is not large enough");

		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

}