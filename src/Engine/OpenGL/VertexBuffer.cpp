#include "VertexBuffer.h"

#include "Log.h"

namespace Engine {

	VertexBuffer::VertexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity), m_Data(), m_Layout()
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
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

	void VertexBuffer::SetData(const std::vector<float>& data)
	{
		ASSERT(data.size() > m_Capacity, "Buffer is not large enough");

		m_Data = data;

		glBufferSubData(GL_ARRAY_BUFFER, 0, m_Data.size() * sizeof(float), m_Data.data());
	}

}