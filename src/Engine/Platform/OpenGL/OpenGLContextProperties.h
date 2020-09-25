#pragma once
#include "Math/Math.h"
#include "Core/RenderContextProperties.h"

namespace Engine {

	struct OpenGLContextProperties : public RenderContextProperties
	{
		virtual RenderingApi GetRenderingApi() const override
		{
			return RenderingApi::OpenGL;
		}

		int VersionMajor = 4;
		int VersionMinor = 5;
		bool CoreProfile = true;
		bool DebugContext = false;
		Vector2 ViewportSize = Vector2(1280, 720);
	};

}