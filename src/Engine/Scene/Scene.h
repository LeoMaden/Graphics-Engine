#pragma once
#include <string>
#include <vector>

#include "Mesh.h"
#include "Node.h"

namespace Engine {

	class Scene
	{
	public:
		std::string Name;
		std::vector<Mesh*> Meshes;
		std::vector<Material*> Materials;

		Node* RootNode;
	};

}