#include "Window.h"
#include "Renderer2D.h"
#include "Log.h"
#include "Utils.h"

#include <iostream>

int main()
{
	Engine::Window* window = new Engine::Window();

	Engine::Log::Init();

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
