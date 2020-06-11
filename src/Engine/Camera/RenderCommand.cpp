#include "RenderCommand.h"

#include <glad/glad.h>

namespace Engine {

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

}