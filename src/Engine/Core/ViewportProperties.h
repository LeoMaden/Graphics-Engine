#pragma once
#include "Math/Math.h"

namespace Engine {

	struct ViewportProperties
	{
		// Size of the viewport in pixels (width, height). 
		// (0, 0) will make the viewport the size of the screen.
		Vector2 Size = Vector2(0, 0); 

		// The position of the top left of the viewport in screen 
		// pixel coords relative to the top left of the screen.
		Vector2 TopLeft = Vector2(0, 0);
	};

}