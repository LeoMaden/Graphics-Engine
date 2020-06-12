#include "VertexArray.h"
#include "Log.h"

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

	VertexArray::VertexArray(VertexBuffer& vbo, IndexBuffer& ibo)
		: m_Vbo(vbo), m_Ibo(ibo)
	{
		glCreateVertexArrays(1, &m_Id);
		glBindVertexArray(m_Id);

		vbo.Bind();
		ibo.Bind();

		const std::vector<VertexBuffer::Layout>& layout = vbo.GetLayout();

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
		glBindVertexArray(m_Id);
	}

}