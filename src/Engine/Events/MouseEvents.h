#pragma once
#include "Event.h"
#include "KeyCodes.h"

namespace Engine {

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(int x, int y)
			: m_X(x), m_Y(y)
		{
		}

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }

	private:
		int m_X;
		int m_Y;
	};

	class MouseDownEvent : public Event
	{
	public:
		MouseDownEvent(KeyCode mouseCode, KeyCode modifier)
			: m_MouseCode(mouseCode)
		{
		}

		KeyCode GetMouseCode()	const { return m_MouseCode; }
		KeyCode GetModifier()	const { return m_Modifier; }

	private:
		KeyCode m_MouseCode;
		KeyCode m_Modifier;
	};

}