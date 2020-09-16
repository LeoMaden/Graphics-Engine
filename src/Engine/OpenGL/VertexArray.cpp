#include "VertexArray.h"
#include "Log.h"

#include "OpenGLUtils.h"

namespace Engine {

	static uint32_t GetTypeSize(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_INT:			return sizeof(int);
		}

		ASSERT(false, "Unsupported type");
		return 0;
	}

	VertexArray::VertexArray(VertexBuffer* vbo, IndexBuffer* ibo)
		: m_Vbo(vbo), m_Ibo(ibo)
	{
		glCreateVertexArrays(1, &m_Id);
		glBindVertexArray(m_Id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo->GetId());
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->GetId());

		const std::vector<VertexBuffer::Layout>& layout = vbo->GetLayout();

		int stride = 0;
		int offset = 0;

		for (auto& elem : layout)
		{
			stride += GetTypeSize(elem.Type) * elem.Count;
		}

		for (auto& elem : layout)
		{
			glEnableVertexArrayAttrib(m_Id, elem.Location);
			glVertexAttribPointer(elem.Location, elem.Count, elem.Type, 
				GL_FALSE, stride, (const void*)offset);

			offset += GetTypeSize(elem.Type) * elem.Count;
		}
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArray::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo->GetId());
		glBindVertexArray(m_Id);
	}

	void VertexArray::Draw(DrawMode mode, uint32_t indexCount) const
	{
		GLenum glMode = GetGLDrawMode(mode);

		// Bind IndexBuffer and VertexArray.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo->GetId());
		glBindVertexArray(m_Id);

		glDrawElements(glMode, indexCount, GL_UNSIGNED_INT, 0);
	}

}