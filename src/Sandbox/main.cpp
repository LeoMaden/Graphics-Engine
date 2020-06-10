#include "Application.h"
#include "Log.h"
#include "Renderer2D.h"

#include <iostream>

class SandboxApp : public Engine::Application
{
public:
	virtual void OnStartup() override
	{
		LOG_DEBUG("App startup");
		Engine::Renderer2D::Init();
	}

	virtual void OnUpdate(float timestep) override
	{
		Engine::Renderer2D::DrawSquare();
	}
};

int main()
{
	SandboxApp* app = new SandboxApp();

	app->Run();

	delete app;

	LOG_DEBUG("Main end");
	std::cin.get();
	return 0;
}


