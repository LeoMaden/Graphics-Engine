#include "Utils.h"
#include "Log.h"

#include <fstream>
#include <sstream>
#undef LoadImage

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine {

	std::string Utils::LoadFile(const std::string& path)
	{
		std::stringstream buffer;
		std::string line;

		std::fstream file;
		file.open(path);

		ASSERT(file, "Could not open file {}", path);

		while (getline(file, line))
		{
			buffer << line << "\n";
		}
		file.close();

		return buffer.str();
	}

	uint8_t* Utils::LoadImage(const std::string& path, ImageProps* props)
	{
		stbi_set_flip_vertically_on_load(true);
		uint8_t* buf = stbi_load(path.c_str(), &props->Width, &props->Height, &props->Channels, 0);

		ASSERT(buf, "Could not open image");
		if (!buf) return nullptr;

		return buf;
	}

	void Utils::FreeImage(uint8_t* data)
	{
		stbi_image_free(data);
	}


	// Helper functions for loading from assimp.

	static glm::vec3 ConvertColor(aiColor3D col)
	{
		return glm::vec3(col.r, col.g, col.b);
	}

	static glm::vec3 ConvertVec3(aiVector3D vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static glm::vec2 ConvertVec2(aiVector3D vec)
	{
		return glm::vec2(vec.x, vec.y);
	}

	static glm::mat4 ConvertMat4(aiMatrix4x4 mat)
	{
		return glm::mat4(
			glm::vec4(mat.a1, mat.a2, mat.a3, mat.a4),
			glm::vec4(mat.b1, mat.b2, mat.b3, mat.b4),
			glm::vec4(mat.c1, mat.c2, mat.c3, mat.c4),
			glm::vec4(mat.d1, mat.d2, mat.d3, mat.d4)
		);
	}

	static std::string ConvertString(aiString str)
	{
		return std::string(str.C_Str());
	}


	static Material* ConvertMaterial(aiMaterial* aiMaterial)
	{
		Material* material = new Material;
		material->Name = std::string(aiMaterial->GetName().C_Str());

		// Default material values.
		aiColor3D aiAmbient(1, 0, 1);
		aiColor3D aiDiffuse(1, 0, 1);
		aiColor3D aiSpecular(1, 0, 1);
		float shininess = 0;
		aiString diffuseTexPath("");
		aiString specularMapPath("");
		aiString shininessMapPath("");

		if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient))
		{
			LOG_WARN("Could not load ambient color for {}", material->Name);
		}

		if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse))
		{
			LOG_WARN("Could not load diffuse color for {}", material->Name);
		}

		if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular))
		{
			LOG_WARN("Could not load specular color for {}", material->Name);
		}

		if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_SHININESS, shininess))
		{
			LOG_WARN("Could not load shininess for {}", material->Name);
		}

		if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexPath);
		}

		if (aiMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &specularMapPath);
		}

		if (aiMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
		{
			aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &shininessMapPath);
		}

		material->AmbientColor = ConvertColor(aiAmbient);
		material->DiffuseColor = ConvertColor(aiDiffuse);
		material->SpecularColor = ConvertColor(aiSpecular);
		material->Shininess = shininess;
		material->DiffuseMapPath = ConvertString(diffuseTexPath);
		material->SpecularMapPath = ConvertString(specularMapPath);
		material->ShininessMapPath = ConvertString(shininessMapPath);

		return material;
	}

	static Mesh* ConvertMesh(aiMesh* aiMesh, const std::vector<Material*>& materials)
	{
		Mesh* mesh = new Mesh;
		mesh->Name = std::string(aiMesh->mName.C_Str());

		// Add vertices to mesh.
		uint32_t nVertices = aiMesh->mNumVertices;

		bool texCoords = aiMesh->mTextureCoords[0] != nullptr;

		for (uint32_t i = 0; i < nVertices; i++)
		{
			Vertex v;
			v.Position = ConvertVec3(aiMesh->mVertices[i]);
			v.Normal = ConvertVec3(aiMesh->mNormals[i]);

			if (texCoords == true)
			{
				v.TexCoord = ConvertVec2(aiMesh->mTextureCoords[0][i]); // Only using first UV for each vertex, could be more?
			}
			else
			{
				v.TexCoord = glm::vec3(0.0f);
			}

			mesh->Vertices.push_back(v);
		}

		// Add indices to mesh.
		uint32_t nFaces = aiMesh->mNumFaces;

		for (uint32_t i = 0; i < nFaces; i++)
		{
			if (aiMesh->mFaces[i].mNumIndices != 3)
			{
				LOG_WARN("Mesh {}: Face {} not triangulated, skipped", mesh->Name, i);
				continue;
			}

			mesh->Indices.push_back(aiMesh->mFaces[i].mIndices[0]);
			mesh->Indices.push_back(aiMesh->mFaces[i].mIndices[1]);
			mesh->Indices.push_back(aiMesh->mFaces[i].mIndices[2]);
		}

		// Add material to mesh.
		uint32_t materialIndex = aiMesh->mMaterialIndex;
		mesh->Material = materials[materialIndex];


		return mesh;
	}

	static Node* ConvertNode(aiNode* aiNode, Node* parent, const std::vector<Mesh*>& meshes)
	{
		Node* node = new Node;
		node->Name = aiNode->mName.C_Str();

		// Set parent to what is provided.
		node->Parent = parent;
		
		// Set Node transform.
		node->Transform = ConvertMat4(aiNode->mTransformation);

		// Recursively convert children to Node using node above as parent.
		uint32_t nChildren = aiNode->mNumChildren;
		node->Children.resize(nChildren);

		for (uint32_t i = 0; i < nChildren; i++)
		{
			Node* child = new Node;

			child = ConvertNode(aiNode->mChildren[i], node, meshes);

			node->Children[i] = child;
		}

		// Add meshes to node.
		uint32_t nMeshes = aiNode->mNumMeshes;
		node->Meshes.resize(nMeshes);

		for (uint32_t i = 0; i < nMeshes; i++)
		{
			uint32_t meshIndex = aiNode->mMeshes[i];

			node->Meshes[i] = meshes[meshIndex];
		}


		return node;
	}



	Scene* Utils::LoadScene(const std::string& path)
	{
		// TODO: Loading options.
		Assimp::Importer importer;
		const aiScene* aiScene = importer.ReadFile(path, aiProcess_Triangulate);

		if (aiScene == nullptr)
		{
			LOG_ERROR("Error loading model {}: {}", path, importer.GetErrorString()); 
			return nullptr;
		}

		Scene* scene = new Scene;

		// Populate Materials. Must be done before meshes as Mesh references Material.
		uint32_t nMaterials = aiScene->mNumMaterials;
		scene->Materials.resize(nMaterials);

		for (uint32_t i = 0; i < nMaterials; i++)
		{
			aiMaterial* aiMaterial = aiScene->mMaterials[i];
			Material* material = ConvertMaterial(aiMaterial);

			// Convert relative texture path to absolute one.
			if ((material->DiffuseMapPath != "") && (material->DiffuseMapPath[0] != 'C'))
			{
				material->DiffuseMapPath = path + "/../" + material->DiffuseMapPath;
			}
			if ((material->SpecularMapPath != "") && (material->SpecularMapPath[0] != 'C'))
			{
				material->SpecularMapPath = path + "/../" + material->SpecularMapPath;
			}

			scene->Materials[i] = material;
		}

		// Populate Meshes.
		uint32_t nMeshes = aiScene->mNumMeshes;
		scene->Meshes.resize(nMeshes);

		for (uint32_t i = 0; i < nMeshes; i++)
		{
			aiMesh* aiMesh = aiScene->mMeshes[i];
			Mesh* mesh = ConvertMesh(aiMesh, scene->Materials);

			scene->Meshes[i] = mesh;
		}

		// Populate Nodes recursively.
		scene->RootNode = ConvertNode(aiScene->mRootNode, nullptr, scene->Meshes);


		return scene;
	}



}