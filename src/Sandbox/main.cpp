#include "Core/Application.h"
#include "Core/CurrentContext.h"
#include "Platform/OpenGL/OpenGLContextProperties.h"

#include <glad/glad.h>

class App : public Engine::Application
{
public:
	App(const Engine::ApplicationProperties& props) 
		: Application(props)
	{
	}

protected:
	virtual void OnUpdate(float ts) override
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);
	}
};

int main()
{
	using namespace Engine;
	Log::Init();

	OpenGLContextProperties glProps;
	glProps.DebugContext = true;
	glProps.ViewportSize = { 800, 600 };

	ApplicationProperties appProps;
	appProps.ContextProps = &glProps;

	Application::Create<App>(appProps);
	CurrentContext::Get()->SetViewPort(ViewportProperties());
	Application::Start();

	Application::Destroy();

	return 0;
}


