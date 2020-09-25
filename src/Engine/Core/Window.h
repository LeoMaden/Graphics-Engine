#pragma once
#include "Pch.h"
#include "Math/Math.h"
#include "Events/Event.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "RenderContext.h"

namespace Engine {

	class Window 
	{
	public:
		struct Properties
		{
			// Size of content area in pixels (width, height).
			Vector2 Size = Vector2(1280, 720);	

			// Position of centre of window in pixels.
			Vector2 Position = Vector2();	

			// The text displayed at the top of the window.
			String Text = "New Window";				
		};


	// Static.
	public:
		static Window* Create(Properties props);

	// Instance.
	public:
		using CallbackFunc = std::function<void(Event&)>;
		virtual void SetCallback(CallbackFunc func) = 0;

		virtual RenderContext* CreateRenderContext(const RenderContextProperties& props) = 0;
		virtual void DeleteRenderContext(RenderContext* context) = 0;

		virtual void SwapBuffers() const = 0;
		virtual void PollEvents() const = 0;

		virtual void Show() const = 0;
		virtual void Hide() const = 0;

		virtual bool IsOpen() const = 0;
		virtual Vector2 GetClientSize() const = 0;

	};

}