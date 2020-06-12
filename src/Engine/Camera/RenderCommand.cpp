#include "RenderCommand.h"

#include <glad/glad.h>

namespace Engine {

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommand::EnableBlending(bool setting)
	{
		if (setting == false)
		{
			glDisable(GL_BLEND);
			return;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}