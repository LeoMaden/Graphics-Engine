#pragma once
#include "Event.h"
#include "Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Engine {

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const glm::vec2& pos, ModifierKeys modifier)
			: m_Position(pos), m_Modifier(modifier)
		{
		}

		glm::vec2		GetPosition()	const { return m_Position; }
		ModifierKeys	GetModifier()	const { return m_Modifier; }

	private:
		glm::vec2		m_Position;
		ModifierKeys	m_Modifier;
	};

	class MouseDownEvent : public Event
	{
	public:
		MouseDownEvent(KeyCode mouseCode, const glm::vec2& pos, ModifierKeys modifier)
			: m_MouseCode(mouseCode), m_Position(pos), m_Modifier(modifier)
		{
		}

		KeyCode			GetMouseCode()	const { return m_MouseCode; }
		glm::vec2		GetPosition()	const { return m_Position; }
		ModifierKeys	GetModifier()	const { return m_Modifier; }

	private:
		KeyCode			m_MouseCode;
		glm::vec2		m_Position;
		ModifierKeys	m_Modifier;
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