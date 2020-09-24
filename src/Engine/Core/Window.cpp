#include "Pch.h"

#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine {

	Window* Window::Create(Properties props)
	{
		return new WindowsWindow(props);
	}

}