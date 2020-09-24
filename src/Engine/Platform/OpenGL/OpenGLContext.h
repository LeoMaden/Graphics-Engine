#pragma once
#include "OpenGLContextProperties.h"

namespace Engine {

	class OpenGLContext
	{
	public:
		OpenGLContext(void* handle, const OpenGLContextProperties& props)
			:m_Handle(handle), m_Properties(props)
		{
		}

		void* GetHandle() const { return m_Handle; }
		const OpenGLContextProperties& GetProperties() const { return m_Properties; }

	private:
		void* m_Handle;
		OpenGLContextProperties m_Properties;
	};

}