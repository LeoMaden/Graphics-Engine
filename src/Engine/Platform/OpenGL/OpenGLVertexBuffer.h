#pragma once
#include "Rendering/VertexBuffer.h"

#include "glad/glad.h"

namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, uint32_t size, BufferUsage usage);
		~OpenGLVertexBuffer();

		virtual int GetId() const override { return m_VaoId; }

		virtual void SetLayout(const VertexBufferLayout& layout) override;
		virtual VertexBufferLayout GetLayout() const override;

		virtual void SetData(const void* data, uint32_t size) override;

	private:
		GLuint m_VboId;
		GLuint m_VaoId;
		VertexBufferLayout m_Layout;
	};

}