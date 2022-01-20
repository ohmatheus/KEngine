#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class GLShader;

//----------------------------------------------------------
struct	SVerticesArrayProperties
{
	unsigned int	m_nRVertices = 0;
	const float*	m_vertices = nullptr;
	bool			m_hasPosition = true;
	bool			m_hasNormal = true;
	bool			m_hasTexCoord = true;
};

//----------------------------------------------------------
class Model
{
public:
	Model()
	{}

	void	LoadModel(std::string const& path);
	void	LoadMeshFromVertexArray(SVerticesArrayProperties const&, std::vector<Texture> const&); // create only 1 mesh from vertex array

	void	Draw(GLShader* shader);

private:
	void					_ProcessNode(aiNode* node, const aiScene* scene);
	Mesh					_ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture>	_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	// model data
	std::vector<Mesh>	m_meshes;
	std::string			m_directory;
};

