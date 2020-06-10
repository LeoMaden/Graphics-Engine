#pragma once
#include "Event.h"
#include "KeyCodes.h"

namespace Engine {

	//enum class KeyFlags
	//{
	//	None = 0,
	//	AltDown = 1 << 0,
	//	Extended = 1 << 1
	//};

	class KeyDownEvent : public Event
	{
	public:
		KeyDownEvent(KeyCode code)//, KeyFlags flags = KeyFlags::None)
			: m_KeyCode(code)//, m_Flags(flags)
		{
		}

		KeyCode		GetKeyCode()	const { return m_KeyCode; }
		//KeyFlags	GetFlags()		const { return m_Flags; }
		//bool		TestFlags(KeyFlags flags) const { return (m_Flags & flags) != KeyFlags::None; }

	private:
		KeyCode		m_KeyCode;
		//KeyFlags	m_Flags;
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