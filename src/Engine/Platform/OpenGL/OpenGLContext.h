#pragma once
#include "OpenGLContextProperties.h"
#include "Core/RenderContext.h"
#include "Core/Window.h"

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(void* handle, const OpenGLContextProperties& props, const Window* parent);

		virtual RenderingApi GetRenderingApi() const override { return RenderingApi::OpenGL; }
		virtual void* GetHandle() const override { return m_Handle; }
		virtual const OpenGLContextProperties& GetProperties() const override { return m_Properties; }

		virtual void SetViewPort(ViewportProperties props) const override;

	private:
		void CreateDebugCallback() const;

	private:
		void* m_Handle;
		OpenGLContextProperties m_Properties;
		const Window* m_ParentWindow;
	};

}