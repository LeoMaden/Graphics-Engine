#pragma once
#include "Event.h"
#include "KeyCodes.h"

#include <glm/glm.hpp>

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
			: m_MouseCode(mouseCode), m_Modifier(modifier)
		{
		}

		KeyCode GetMouseCode()	const { return m_MouseCode; }
		KeyCode GetModifier()	const { return m_Modifier; }

	private:
		KeyCode m_MouseCode;
		KeyCode m_Modifier;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(int distance, ModifierKeys modifier, const glm::vec2& pos)
			: m_Dist(distance), m_Modifier(modifier), m_Pos(pos)
		{
		}

		int				GetDistance() const { return m_Dist; }
		ModifierKeys	GetModifier() const { return m_Modifier; }
		glm::vec2		GetPosition() const { return m_Pos; }

	private:
		int				m_Dist;
		ModifierKeys	m_Modifier;
		glm::vec2		m_Pos;
	};

}