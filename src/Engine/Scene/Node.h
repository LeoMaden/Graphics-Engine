#pragma once
#include <vector>
#include <string>

#include "Mesh.h"

namespace Engine {

	struct Node
	{
		std::string Name;

		Node* Parent;
		glm::mat4 Transform;
		std::vector<Node*> Children;

		std::vector<Mesh*> Meshes;
	};

}