#include "Window.h"

#include <iostream>

int main()
{
	Engine::Window* window = new Engine::Window();

	window->Create();
	window->CreateContext();

	while (!window->ShouldClose) 
	{
		window->SwapBuffers();
		window->PollEvents();
	}

	delete window;

	return 0;
}
