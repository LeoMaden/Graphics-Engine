#include "Window.h"
#include "Renderer2D.h"

#include <iostream>

int main()
{
	Engine::Window* window = new Engine::Window();

	window->Create();
	window->CreateContext();

	Engine::Renderer2D::Setup();

	while (!window->ShouldClose) 
	{
		Engine::Renderer2D::DrawSquare();

		window->SwapBuffers();
		window->PollEvents();
	}

	delete window;

	return 0;
}
