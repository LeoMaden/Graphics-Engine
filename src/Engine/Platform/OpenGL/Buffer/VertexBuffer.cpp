#include "Pch.h"
#include "VertexBuffer.h"
#include "OpenGL/OpenGLUtils.h"

#include "Log.h"

namespace Engine {

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
		: m_Id(0), m_Capacity(size), m_Layout(), m_Type(BufferType::Static)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}


	VertexBuffer::VertexBuffer(uint32_t capacity)
		: m_Id(0), m_Capacity(capacity), m_Layout(), m_Type(BufferType::Dynamic)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		ASSERT(size <= m_Capacity, "Buffer is not large enough");
		ASSERT(m_Type == BufferType::Dynamic, "Buffer must be dynamic");

		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}


	void VertexBuffer::AddLayout(uint32_t location, DataType type, uint32_t count)
	{
		GLenum glType = GetGLType(type);

		m_Layout.push_back({ location, glType, count });
	}

}