#include "Pch.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLBufferUsage.h"
#include "OpenGLTypeInfo.h"

namespace Engine {

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_VboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GetGLBufferUsage(usage));

		glCreateVertexArrays(1, &m_VaoId);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_VboId);
		glDeleteVertexArrays(1, &m_VaoId);
	}

	void OpenGLVertexBuffer::SetLayout(const VertexBufferLayout& layout)
	{
		m_Layout = layout;

		glBindVertexArray(m_VaoId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

		int stride = 0;
		int offset = 0;

		for (auto& elem : layout.Elements)
		{
			stride += GetShaderDataTypeSize(elem.Type);
		}

		for (auto& elem : layout.Elements)
		{
			GLTypeInfo info = GetGLTypeInfo(elem.Type);

			glEnableVertexArrayAttrib(m_VaoId, elem.Location);
			glVertexAttribPointer(elem.Location, info.Size, info.Type,
				GL_FALSE, stride, (const void*)offset);

			offset += GetShaderDataTypeSize(elem.Type);
		}
	}

	VertexBufferLayout OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

}