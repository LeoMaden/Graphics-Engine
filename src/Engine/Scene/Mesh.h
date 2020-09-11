#pragma once
#include <string>
#include <vector>

#include "Vertex.h"
#include "Material.h"

namespace Engine {

	class Mesh
	{
	public:
		std::string Name;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		Material* Material;
	};

}