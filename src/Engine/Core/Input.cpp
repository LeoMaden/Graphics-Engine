#include "Pch.h"
#include "Input.h"

namespace Engine {

	/*static*/ void Input::SetProvider(InputProvider* provider)
	{
		s_Provider = provider;
	}

	/*static*/ KeyState Input::GetKeyState(KeyCode key)
	{
		return s_Provider->GetKeyState(key);
	}

	/*static*/ Vector2 Input::GetMousePosition()
	{
		return s_Provider->GetMousePosition();
	}

	/*static*/ InputProvider* Input::s_Provider;

}