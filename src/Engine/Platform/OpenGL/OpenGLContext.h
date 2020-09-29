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

		virtual void SetVertexBuffer(VertexBuffer& buffer) override;
		virtual VertexBuffer* GetVertexBuffer() override;

		virtual void SetIndexBuffer(IndexBuffer& buffer) override;
		virtual IndexBuffer* GetIndexBuffer() override;

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;
		virtual PrimitiveTopology GetPrimitiveTopology() const override;

		virtual void Draw(uint32_t count) const override;
		virtual void DrawIndexed(uint32_t count) const override;

	private:
		void CreateDebugCallback() const;

	private:
		void* m_Handle;
		OpenGLContextProperties m_Properties;
		const Window* m_ParentWindow;

		VertexBuffer* m_VertexBuffer = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
		PrimitiveTopology m_Primitive = PrimitiveTopology::TriangleList;
	};

}