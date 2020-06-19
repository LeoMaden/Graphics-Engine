#include "OpenGLUtils.h"
#include "Log.h"

namespace Engine {

	GLenum GetGLDrawMode(DrawMode mode)
	{
		switch (mode)
		{
		case Engine::DrawMode::Points:		return GL_POINTS;
		case Engine::DrawMode::Lines:		return GL_LINES;
		case Engine::DrawMode::Triangles:	return GL_TRIANGLES;
		}
		ASSERT(false, "Invalid draw mode");
	}

	GLenum GetGLType(DataType type)
	{
		switch (type)
		{
		case Engine::DataType::Int16:		return GL_SHORT;
		case Engine::DataType::Int32:		return GL_INT;
		case Engine::DataType::UInt16:		return GL_UNSIGNED_SHORT;
		case Engine::DataType::UInt32:		return GL_UNSIGNED_INT;
		case Engine::DataType::Float32:		return GL_FLOAT;
		case Engine::DataType::Double64:	return GL_DOUBLE;
		}
		ASSERT(false, "Invalid type");
	}

}