#include "Pch.h"
#include "OpenGLContext.h"
#include "OpenGLPrimitiveTopology.h"
#include "OpenGLTypeInfo.h"

#include <glad/glad.h>

namespace Engine {

	OpenGLContext::OpenGLContext(void* handle, const OpenGLContextProperties& props, const Window* parent)
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

	void OpenGLContext::SetVertexBuffer(VertexBuffer& buffer)
	{
		glBindVertexArray(buffer.GetId());
		m_VertexBuffer = &buffer;
	}

	VertexBuffer* OpenGLContext::GetVertexBuffer()
	{
		return m_VertexBuffer;
	}

	void OpenGLContext::SetIndexBuffer(IndexBuffer& buffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->GetId());
	}

	IndexBuffer* OpenGLContext::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}

	void OpenGLContext::SetPrimitiveTopology(PrimitiveTopology topology)
	{
		m_Primitive = topology;
	}

	PrimitiveTopology OpenGLContext::GetPrimitiveTopology() const
	{
		return m_Primitive;
	}

	void OpenGLContext::Draw(uint32_t count) const
	{
		glDrawArrays(GetGLPrimitiveTopology(m_Primitive), 0, count);
	}

	void OpenGLContext::DrawIndexed(uint32_t count) const
	{
		ASSERT(m_IndexBuffer, "Index buffer must be set");
		glDrawElements(GetGLPrimitiveTopology(m_Primitive), count, GetGLTypeInfo(m_IndexBuffer->GetIndexDataType()).Type, nullptr);
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