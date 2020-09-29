#pragma once
#include "Rendering/PrimitiveTopology.h"
#include "Log.h"

#include <glad/glad.h>

namespace Engine {

	GLenum GetGLPrimitiveTopology(PrimitiveTopology primitive)
	{
		switch (primitive)
		{
		case Engine::PrimitiveTopology::PointList:		return GL_POINTS;
		case Engine::PrimitiveTopology::LineList:		return GL_LINES;
		case Engine::PrimitiveTopology::TriangleList:	return GL_TRIANGLES;
		case Engine::PrimitiveTopology::LineStrip:		return GL_LINE_STRIP;
		case Engine::PrimitiveTopology::TriangleStrip:	return GL_TRIANGLE_STRIP;

		default:
			ASSERT(false, "Topology not supported");
			return GL_NONE;
		}
	}

}