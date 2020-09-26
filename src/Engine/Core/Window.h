#pragma once
#include "Pch.h"
#include "Math/Math.h"
#include "Events/Event.h"
#include "Core/WindowProperties.h"
#include "Core/InputProvider.h"
#include "Core/RenderContext.h"

namespace Engine {

	class Window : public InputProvider
	{
	// Static.
	public:
		static Window* Create(const WindowProperties& props);

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