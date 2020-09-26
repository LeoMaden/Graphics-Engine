#include "Pch.h"
#include "CurrentContext.h"

namespace Engine {

	RenderContext* CurrentContext::Get()
	{
		return s_CurrentContext;
	}

	RenderContext* CurrentContext::s_CurrentContext;

	void CurrentContext::Set(RenderContext* context)
	{
		s_CurrentContext = context;
	}


}