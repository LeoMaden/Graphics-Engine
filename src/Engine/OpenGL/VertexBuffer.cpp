#include "VertexBuffer.h"

namespace Engine {

	VertexBuffer::VertexBuffer(const std::vector<float>& data)
		: m_Id(0), m_Data(data)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), m_Data.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void VertexBuffer::AddLayout(uint32_t location, GLenum type, uint32_t count)
	{
		m_Layout.push_back({ location, type, count });
	}

}