#include "Pch.h"
#include "IndexBuffer.h"
#include "Core/CurrentContext.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Engine {

	IndexBuffer* IndexBuffer::Create(const void* indices, uint32_t size, BufferUsage usage)
	{
		switch (CurrentContext::Get()->GetRenderingApi())
		{
		case RenderingApi::OpenGL:
		{
			return new OpenGLIndexBuffer(indices, size, usage);
		}
		default:
		{
			return nullptr;
		}
		}
	}

}