#pragma once
#include "Core/Core.h"

namespace Engine {
	
	struct WindowProperties
	{
		// Size of content area in pixels (width, height).
		Vector2 Size = Vector2(1280, 720);

		// Position of centre of window in pixels.
		Vector2 Position = Vector2();

		// The text displayed at the top of the window.
		String Text = "New Window";
	};

}