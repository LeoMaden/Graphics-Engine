#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Engine {

	class VertexArray
	{
	public:
		VertexArray(VertexBuffer* vbo, IndexBuffer* ibo);
		~VertexArray();

		uint32_t GetId() const { return m_Id; }

		void Bind() const;
		void Draw(DrawMode mode, uint32_t indexCount) const;

	private:
		GLuint			m_Id;
		VertexBuffer*	m_Vbo;
		IndexBuffer*	m_Ibo;
	};

}