#include "Pch.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLBufferUsage.h"

namespace Engine {

	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GetGLBufferUsage(usage));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGLIndexBuffer::SetIndices(const void* indices, uint32_t size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
	}

}