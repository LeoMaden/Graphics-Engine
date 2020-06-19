#include "RenderCommand.h"
#include "Log.h"
#include "OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace Engine {

	void RenderCommand::InitRenderer()
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			LOG_INFO("Open GL Debug enabled");

			auto callback = [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				if (severity == GL_DEBUG_SEVERITY_HIGH)
				{
					LOG_ERROR("Open GL: {}", message);
				}
				else
				{
					LOG_WARN("Open GL: {}", message);
				}
			};

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(callback, nullptr);
		}
		else
		{
			LOG_INFO("Open GL Debug not enabled");
		}
	}

	void RenderCommand::DrawIndexed(DrawMode mode, uint32_t count, DataType type, uint32_t offset)
	{
		GLenum glType = GetGLType(type);
		GLenum glMode = GetGLDrawMode(mode);

		glDrawElements(glMode, count, glType, (const void*)offset);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::Clear(const glm::vec4& color)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	void RenderCommand::EnableDepth(bool setting)
	{
		if (setting == false)
		{
			glDisable(GL_DEPTH_TEST);
			return;
		}

		glEnable(GL_DEPTH_TEST);
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