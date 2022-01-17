#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class GLShader;

//----------------------------------------------------------
class Model
{
public:
	Model(std::string const& path)
	{
		LoadModel(path);
	}
	void Draw(GLShader* shader);

private:
	// model data
	std::vector<Mesh>	m_Meshes;
	std::string			m_Directory;

	void					LoadModel(std::string const& path);
	void					ProcessNode(aiNode* node, const aiScene* scene);
	Mesh					ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture>	LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};