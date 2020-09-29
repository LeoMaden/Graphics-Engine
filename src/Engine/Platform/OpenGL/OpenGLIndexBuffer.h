#pragma once
#include "Rendering/BufferUsage.h"
#include "Rendering/IndexBuffer.h"

#include <glad/glad.h>

namespace Engine {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const void* indices, uint32_t size, BufferUsage usage);
		~OpenGLIndexBuffer();

		virtual int GetId() const override { return m_Id; }

		virtual void SetIndexDataType(ShaderDataType type) override { m_IndexType = type; }
		virtual ShaderDataType GetIndexDataType() const override { return m_IndexType; }

		virtual void SetIndices(const void* indices, uint32_t size) override;

	private:
		GLuint m_Id;
		ShaderDataType m_IndexType;
	};

}