#include "Pch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Engine {

	OpenGLContext::OpenGLContext(HGLRC handle, const OpenGLContextProperties& props)
		:m_Handle(handle), m_Properties(props)
	{
		if (props.DebugContext == true)
		{
			CreateDebugCallback();
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