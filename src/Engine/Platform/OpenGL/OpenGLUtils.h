#pragma once

#include <glad/glad.h>

#include "Rendering/RenderUtils.h"

namespace Engine {

	GLenum GetGLDrawMode(DrawMode mode);

	GLenum GetGLType(DataType type);

}