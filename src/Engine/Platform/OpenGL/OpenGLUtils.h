#pragma once

#include <glad/glad.h>

#include "Renderer/RenderUtils.h"

namespace Engine {

	GLenum GetGLDrawMode(DrawMode mode);

	GLenum GetGLType(DataType type);

}