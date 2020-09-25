#pragma once
#include "OpenGLContextProperties.h"
#include "Core/RenderContext.h"

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(void* handle, const OpenGLContextProperties& props)
			:m_Handle(handle), m_Properties(props)
		{
		}

		virtual RenderingApi GetRenderingApi() const override { return RenderingApi::OpenGL; }
		virtual void* GetHandle() const override { return m_Handle; }
		virtual const OpenGLContextProperties& GetProperties() const override { return m_Properties; }

	private:
		void* m_Handle;
		OpenGLContextProperties m_Properties;
	};

}