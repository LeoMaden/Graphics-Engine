#include <windows.h>
#include "Window.h"

int main()
{
    Engine::Window* window = new Engine::Window();

    window->Create();
    window->RunSystemEventLoop();

    delete window;

    return 0;
}
