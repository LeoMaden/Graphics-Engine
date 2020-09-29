#pragma once
#include "ShaderDataType.h"
#include "Core/Core.h"

namespace Engine {

	struct VertexBufferLayout
	{
		struct Element
		{
			String Name;
			int Location;
			ShaderDataType Type;
		};

		List<Element> Elements;

		void AddElement(Element& e) { Elements.push_back(e); }

	};

}