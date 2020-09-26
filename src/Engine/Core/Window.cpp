#include "Pch.h"

#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine {

	Window* Window::Create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

}