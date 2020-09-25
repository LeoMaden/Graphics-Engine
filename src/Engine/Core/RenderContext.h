#pragma once
#include "RenderContextProperties.h"
#include "ViewportProperties.h"

namespace Engine {

	class RenderContext
	{
	public:
		virtual RenderingApi GetRenderingApi() const = 0;
		virtual void* GetHandle() const = 0;
		virtual const RenderContextProperties& GetProperties() const = 0;

		virtual void SetViewPort(ViewportProperties props) const = 0;
	};

}