#include "Core/Window.h"
#include "Pch.h"

#include "Platform/OpenGL/OpenGLContextProperties.h"

#include <glad/glad.h>


int main()
{
	using namespace Engine;
	Log::Init();

	Window* window = Window::Create(Window::Properties());
	window->CreateOpenGLContext(OpenGLContextProperties());

	Vector2 size = window->GetClientSize();
	LOG_WARN("Size: {}, {}", size.x, size.y);

	while (window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);

		window->PollEvents();
		window->SwapBuffers();
	}

	delete window;

	return 0;
}


