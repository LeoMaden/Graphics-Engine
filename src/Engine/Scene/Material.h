#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Engine {

	struct Material
	{
		std::string Name;

		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;

		float Shininess; // Specular exponent.
	};

}