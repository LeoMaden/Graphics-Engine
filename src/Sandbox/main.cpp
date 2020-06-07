#include "Window.h"
#include "Renderer2D.h"
#include "Log.h"
#include "Utils.h"

#include <iostream>

int main()
{
	Engine::Log::Init();

	Engine::Window* window = new Engine::Window();

	window->Create();
	window->CreateContext();

	Engine::Renderer2D::Init();

	while (!window->ShouldClose) 
	{
		Engine::Renderer2D::DrawSquare();

		window->SwapBuffers();
		window->PollEvents();
	}

	delete window;

	return 0;
}
