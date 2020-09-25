#pragma once
#include "OpenGLContextProperties.h"
#include "Core/RenderContext.h"

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(HGLRC handle, const OpenGLContextProperties& props);

		virtual RenderingApi GetRenderingApi() const override { return RenderingApi::OpenGL; }
		virtual void* GetHandle() const override { return m_Handle; }
		virtual const OpenGLContextProperties& GetProperties() const override { return m_Properties; }

	private:
		void CreateDebugCallback() const;

	private:
		HGLRC m_Handle;
		OpenGLContextProperties m_Properties;
	};

}