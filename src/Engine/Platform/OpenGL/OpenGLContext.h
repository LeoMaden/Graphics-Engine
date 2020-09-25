#pragma once
#include "OpenGLContextProperties.h"
#include "Core/RenderContext.h"

namespace Engine {

	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(void* handle, const OpenGLContextProperties& props, const Window* const parent);

		virtual RenderingApi GetRenderingApi() const override { return RenderingApi::OpenGL; }
		virtual void* GetHandle() const override { return m_Handle; }
		virtual const OpenGLContextProperties& GetProperties() const override { return m_Properties; }

		virtual void SetViewPort(ViewportProperties props) const override;

	private:
		void CreateDebugCallback() const;

	private:
		void* m_Handle;
		OpenGLContextProperties m_Properties;
		const Window* const m_ParentWindow;
	};

}