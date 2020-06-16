#pragma once

#include <functional>

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
		virtual bool		IsHandled()		const	{ return m_Handled; }
		virtual void		SetHandled(bool b)		{ m_Handled = b; }

	protected:
		bool		m_Handled = false;
	};


	template<typename EventT>
	void DispatchEvent(Engine::Event& e, std::function<void(EventT&)> handler)
	{
		if (typeid(e) == typeid(EventT))
		{
			EventT& specificEvent = static_cast<EventT&>(e);
			handler(specificEvent);
		}
	}



}