#pragma once
#include "KeyState.h"
#include "Core/KeyCodes.h"
#include "Math/Math.h"

namespace Engine {

	class InputProvider
	{
	public:
		virtual KeyState GetKeyState(KeyCode key) const = 0;
		virtual Vector2 GetMousePosition() const = 0;
	};

}