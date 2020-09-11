#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Engine {

	class VertexArray
	{
	public:
		VertexArray(VertexBuffer* vbo, IndexBuffer* ibo);
		~VertexArray();

		void Bind() const;

	private:
		GLuint			m_Id;
		VertexBuffer*	m_Vbo;
		IndexBuffer*	m_Ibo;
	};

}