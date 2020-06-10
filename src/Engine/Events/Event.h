#pragma once

#include "Window.h"

namespace Engine {

	enum class EventType
	{
		None = 0,
		KeyDown, KeyUp,
		MouseDown, MouseUp, MouseMove,
		WindowResize, WindowClose
	};

	class Event
	{
	public:
		virtual EventType	GetType()		const { return m_Type; }
		virtual bool		IsHandled()		const { return m_Handled; }

		virtual void		SetHandled(bool b) { m_Handled = b; }

	protected:
		EventType	m_Type = EventType::None;
		bool		m_Handled = false;
	};

	using Callback = void (*)(Event&);
	using Handler = bool (*)(Event&);



}