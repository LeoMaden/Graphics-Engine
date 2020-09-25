#pragma once
#include "InputProvider.h"

namespace Engine {

	class Input
	{
	public:
		static void SetProvider(InputProvider* provider);

		static KeyState GetKeyState(KeyCode key);
		static Vector2 GetMousePosition();

	private:
		static InputProvider* s_Provider;
	};

}