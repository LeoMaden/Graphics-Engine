#pragma once
#include "Pch.h"

namespace Engine {

	struct OpenGLContextProperties
	{
		int VersionMajor = 4;
		int VersionMinor = 5;
		bool CoreProfile = true;
		bool DebugContext = false;
		Vector2 ViewportSize = Vector2(1280, 720);
	};

}