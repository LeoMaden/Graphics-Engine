#include "Sandbox2D.h"

#include <iostream>



int main()
{
	Sandbox2D* app = new Sandbox2D();

	app->Run();

	delete app;

	LOG_DEBUG("Main end");
	std::cin.get();
	return 0;
}


