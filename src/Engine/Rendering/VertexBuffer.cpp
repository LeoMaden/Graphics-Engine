#include "Pch.h"
#include "VertexBuffer.h"
#include "Core/CurrentContext.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Engine {

	VertexBuffer* VertexBuffer::Create(const void* data, uint32_t size, BufferUsage usage)
	{
		switch (CurrentContext::Get()->GetRenderingApi())
		{
			case RenderingApi::OpenGL:
			{
				return new OpenGLVertexBuffer(data, size, usage);
			}
			default:
			{
				return nullptr;
			}
		}
	}

}