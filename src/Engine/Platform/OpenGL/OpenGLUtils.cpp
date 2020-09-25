#include "Pch.h"
#include "OpenGLUtils.h"
#include "Log.h"

namespace Engine {

	GLenum GetGLDrawMode(DrawMode mode)
	{
		switch (mode)
		{
		case DrawMode::Points:			return GL_POINTS;
		case DrawMode::Lines:			return GL_LINES;
		case DrawMode::Triangles:		return GL_TRIANGLES;
		case DrawMode::LineLoops:		return GL_LINE_LOOP;
		case DrawMode::TriangleStrip:	return GL_TRIANGLE_STRIP;
		}
		ASSERT(false, "Invalid draw mode");
	}

	GLenum GetGLType(DataType type)
	{
		switch (type)
		{
		case DataType::Int16:		return GL_SHORT;
		case DataType::Int32:		return GL_INT;
		case DataType::UInt16:		return GL_UNSIGNED_SHORT;
		case DataType::UInt32:		return GL_UNSIGNED_INT;
		case DataType::Float32:		return GL_FLOAT;
		case DataType::Double64:	return GL_DOUBLE;
		}
		ASSERT(false, "Invalid type");
	}

}