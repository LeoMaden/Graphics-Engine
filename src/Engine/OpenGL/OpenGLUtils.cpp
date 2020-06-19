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
		case Engine::DataType::UInt16:		return GL_UNSIGNED_SHORT;
		case Engine::DataType::UInt32:		return GL_UNSIGNED_INT;
		}
		ASSERT(false, "Invalid type");
	}

}