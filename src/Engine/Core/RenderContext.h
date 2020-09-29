#pragma once
#include "RenderContextProperties.h"
#include "ViewportProperties.h"

#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/BufferUsage.h"
#include "Rendering/PrimitiveTopology.h"

namespace Engine {

	class RenderContext
	{
	public:
		virtual RenderingApi GetRenderingApi() const = 0;
		virtual void* GetHandle() const = 0;
		virtual const RenderContextProperties& GetProperties() const = 0;

		virtual void SetViewPort(ViewportProperties props) const = 0;

		virtual void SetVertexBuffer(VertexBuffer& buffer) = 0;
		virtual VertexBuffer* GetVertexBuffer() = 0;

		virtual void SetIndexBuffer(IndexBuffer& buffer) = 0;
		virtual IndexBuffer* GetIndexBuffer() = 0;

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
		virtual PrimitiveTopology GetPrimitiveTopology() const = 0;

		virtual void Draw(uint32_t count) const = 0;
		virtual void DrawIndexed(uint32_t count) const = 0;
	};

}