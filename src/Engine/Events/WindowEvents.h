#pragma once
#include "Event.h"

namespace Engine {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{
		}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		int m_Width;
		int m_Height;
	};

}