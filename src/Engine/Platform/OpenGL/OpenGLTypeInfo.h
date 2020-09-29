#pragma once
#include "Rendering/ShaderDataType.h"
#include "Log.h"

#include <glad/glad.h>

namespace Engine {

	struct GLTypeInfo
	{
		GLint Size;
		GLenum Type;
	};

	static GLTypeInfo GetGLTypeInfo(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:		return { 1, GL_FLOAT };
		case Engine::ShaderDataType::Int:		return { 1, GL_INT };
		case Engine::ShaderDataType::UInt:		return { 1, GL_UNSIGNED_INT };
		case Engine::ShaderDataType::Int16:		return { 1, GL_SHORT };
		case Engine::ShaderDataType::UInt16:	return { 1, GL_UNSIGNED_SHORT };
		case Engine::ShaderDataType::Float64:	return { 1, GL_DOUBLE };
		case Engine::ShaderDataType::Vector2:	return { 2, GL_FLOAT };
		case Engine::ShaderDataType::Vector3:	return { 3, GL_FLOAT };
		case Engine::ShaderDataType::Vector4:	return { 4, GL_FLOAT };

		default:
			ASSERT(false, "Unsupported data type");
			return { 0, GL_NONE };
		}
	}

}