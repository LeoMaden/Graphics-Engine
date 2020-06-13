#pragma once
#include <glm/glm.hpp>

namespace Engine {

	class Texture2DBase
	{
	public:
		virtual uint32_t GetId() const = 0;
		virtual void Bind(uint32_t slot) const = 0;
		virtual const glm::vec2& GetTexCoord(uint32_t i) const = 0;
	};

}