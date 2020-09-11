#pragma once
#include <vector>
#include <string>

#include "Mesh.h"

namespace Engine {

	struct Node
	{
		std::string Name;

		Node* Parent;
		std::vector<Node*> Children;

		std::vector<Mesh*> Meshes;
	};

}