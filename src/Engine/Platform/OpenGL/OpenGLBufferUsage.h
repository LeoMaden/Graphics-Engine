#pragma once

namespace Engine {

	static GLenum GetGLBufferUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case Engine::BufferUsage::Static:	return GL_STATIC_DRAW;
		case Engine::BufferUsage::Dynamic:	return GL_DYNAMIC_DRAW;
		case Engine::BufferUsage::Default:	return GL_DYNAMIC_DRAW;

		default:
			ASSERT(false, "Buffer usage not supported");
			return GL_NONE;
		}
	}
}