#pragma once
#include "Pch.h"
#include "WindowProperties.h"
#include "RenderContextProperties.h"

namespace Engine {

	struct ApplicationProperties
	{
		String AppName = "Graphics Engine";
		WindowProperties* WindowProps = nullptr;
		RenderContextProperties* ContextProps = nullptr;
	};

}