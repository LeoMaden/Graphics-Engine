#pragma once
#include "Event.h"
#include "KeyCodes.h"

namespace Engine {

	struct KeyFlags
	{
		bool Extended;
	};

	class KeyDownEvent : public Event
	{
	public:
		KeyDownEvent(KeyCode code, KeyFlags flags = KeyFlags())
			: m_KeyCode(code), m_Flags(flags)
		{
		}

		KeyCode		GetKeyCode()	const { return m_KeyCode; }
		KeyFlags	GetFlags()		const { return m_Flags; }

	private:
		KeyCode		m_KeyCode;
		KeyFlags	m_Flags;
	};
	
	class KeyUpEvent : public Event
	{
	public:
		KeyUpEvent(KeyCode code)
			: m_KeyCode(code)
		{
		}

		KeyCode GetKeyCode() const { return m_KeyCode; }

	private:
		KeyCode		m_KeyCode;
	};

}