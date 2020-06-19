#include "RenderCommand.h"
#include "Log.h"
#include "OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace Engine {

	void RenderCommand::DrawIndexed(DrawMode mode, uint32_t count, DataType type, uint32_t offset)
	{
		GLenum glType = GetGLType(type);
		GLenum glMode = GetGLDrawMode(mode);

		glDrawElements(glMode, count, glType, (const void*)offset);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommand::Clear(const glm::vec4& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.r, color.g, color.b, color.a);
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

	void RenderCommand::SetViewport(const glm::vec2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void RenderCommand::SmoothLines(bool setting)
	{
		if (setting == false)
		{
			glDisable(GL_LINE_SMOOTH);
			return;
		}

		glEnable(GL_LINE_SMOOTH);
	}

	void RenderCommand::LineWidth(float thickness)
	{
		glLineWidth(thickness);
	}

	void RenderCommand::PointSize(float size)
	{
		glPointSize(size);
	}

}