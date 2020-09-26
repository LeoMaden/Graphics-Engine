#pragma once
#include "Core/RenderContext.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Engine {

	class CurrentContext
	{
	public:
		static RenderContext* Get();

	protected:
		static RenderContext* s_CurrentContext;

		// Only allow windows to set the current context.
		friend WindowsWindow;
		static void Set(RenderContext* context);
	};

}