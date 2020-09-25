#include "Core/Window.h"
#include "Pch.h"
#include "Core/Input.h"

#include "Platform/OpenGL/OpenGLContextProperties.h"

#include <glad/glad.h>

void EventHandler(Engine::Event& e)
{
	//LOG_TRACE("Event!");
}

int main()
{
	using namespace Engine;
	Log::Init();

	Window* window = Window::Create(Window::Properties());

	OpenGLContextProperties glProps;
	glProps.DebugContext = true;
	glProps.ViewportSize = { 800, 600 };
	RenderContext* glContext = window->CreateRenderContext(glProps);

	window->SetCallback(&EventHandler);

	Input::SetProvider(window);

	while (window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);

		window->PollEvents();
		window->SwapBuffers();
	}

	window->DeleteRenderContext(glContext);

	delete window;
	return 0;
}


