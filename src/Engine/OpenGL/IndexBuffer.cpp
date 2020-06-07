#include "IndexBuffer.h"

namespace Engine {

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
		: m_Id(0), m_Indices(indices)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), m_Indices.data(), GL_STATIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

}