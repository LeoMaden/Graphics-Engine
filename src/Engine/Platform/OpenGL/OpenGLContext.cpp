#include "Pch.h"
#include "OpenGLContext.h"

#include "Core/Window.h"

#include <glad/glad.h>

namespace Engine {

	OpenGLContext::OpenGLContext(void* handle, const OpenGLContextProperties& props, const Window* const parent)
		:m_Handle(handle), m_Properties(props), m_ParentWindow(parent)
	{
		if (props.DebugContext == true)
		{
			CreateDebugCallback();
		}
	}

	void OpenGLContext::SetViewPort(ViewportProperties props) const
	{
		// Make viewport size of screen.
		if (props.Size == Vector2(0, 0))
		{
			Vector2 screenSize = m_ParentWindow->GetClientSize();
			glViewport(0, 0, screenSize.x, screenSize.y);
		}
		else
		{
			glViewport(props.TopLeft.x, props.TopLeft.y, props.Size.x, props.Size.y);
		}
	}

	void OpenGLContext::CreateDebugCallback() const
	{
		LOG_INFO("OpenGL debug messages enabled");

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

}