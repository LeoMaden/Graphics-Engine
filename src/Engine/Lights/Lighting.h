#pragma once
#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"

namespace Engine {

	struct Lighting
	{
		std::vector<DirectionalLight>	DirectionalLights;
		std::vector<PointLight>			PointLights;
	};

}