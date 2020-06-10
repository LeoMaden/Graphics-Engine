#include "Window.h"
#include "Renderer2D.h"
#include "Log.h"
#include "Utils.h"

#include "Events/KeyEvents.h"

#include <iostream>


static void OnKeyDown(Engine::KeyDownEvent& e)
{
	LOG_DEBUG("Key down: {}", e.GetKeyCode());

	e.SetHandled(true);
}

static void OnKeyUp(Engine::KeyUpEvent& e)
{
	LOG_DEBUG("Key up: {}", e.GetKeyCode());
	e.SetHandled(true);
}

template<typename EventT, typename HandlerT>
static void Dispatch(Engine::Event& e, HandlerT& handler)
{
	if (typeid(e) == typeid(EventT))
	{
		EventT& specificEvent = static_cast<EventT&>(e);
		handler(specificEvent);
	}
}


void EventCallback(Engine::Event& e)
{
	Dispatch<Engine::KeyDownEvent>(e, OnKeyDown);
	Dispatch<Engine::KeyUpEvent>(e, OnKeyUp);

	if (e.IsHandled() == false)
	{
		LOG_DEBUG("Unknown event");
	}
}


int main()
{
	Engine::Log::Init();

	Engine::Window* window = new Engine::Window();

	window->Create();
	window->CreateContext();
	window->SetCallback(EventCallback);

	Engine::Renderer2D::Init();

	while (!window->ShouldClose()) 
	{
		Engine::Renderer2D::DrawSquare();

		window->SwapBuffers();
		window->PollEvents();
	}

	delete window;

	return 0;
}


