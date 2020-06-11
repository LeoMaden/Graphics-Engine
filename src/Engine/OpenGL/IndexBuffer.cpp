#include "IndexBuffer.h"

#include "Log.h"

namespace Engine {

	IndexBuffer::IndexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity), m_Indices()
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void IndexBuffer::SetIndices(const std::vector<uint32_t>& indices)
	{
		ASSERT(indices.size() > m_Capacity, "Buffer is not large enough");

		m_Indices = indices;

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices.size() * sizeof(uint32_t), m_Indices.data());
	}

}