#pragma once

namespace Engine {

	enum class RenderingApi
	{
		OpenGL
	};

	class RenderContextProperties
	{
	public:
		virtual RenderingApi GetRenderingApi() const = 0;
	};

}