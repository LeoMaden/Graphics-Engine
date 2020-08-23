#pragma once
#include <glm/glm.hpp>

namespace Engine {

	class Camera
	{
	public:
		virtual const glm::mat4& GetViewProjMat() const = 0;
		virtual const glm::vec3& GetPos() const = 0;

	};

}