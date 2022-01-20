#include "stdafx.h"

#include "Model.h"

#include "Game.h"
#include "RenderSystem.h"
#include "TextureResource.h"


//----------------------------------------------------------
void Model::Draw(GLShader* shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
		m_meshes[i].Draw(shader);
}

//----------------------------------------------------------
void		Model::LoadModel(std::string const& path)
{
	assert(m_meshes.size() == 0);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	_ProcessNode(scene->mRootNode, scene);
}

//----------------------------------------------------------
void		Model::LoadMeshFromVertexArray(SVerticesArrayProperties const & verticeArrayProperties, std::vector<Texture> const & texturesIn)
{
	// we are asserting that vertices* is 8 float per vertices (position, normals, texcoord)
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	textures.insert(textures.end(), texturesIn.begin(), texturesIn.end());

	for (uint vertexId = 0; vertexId < verticeArrayProperties.m_nRVertices; vertexId++)
	{
		float const*	currentVertex = verticeArrayProperties.m_vertices + 8 * vertexId;
		Vertex			vertex;

		vertex.Position.x = currentVertex[0];
		vertex.Position.y = currentVertex[1];
		vertex.Position.z = currentVertex[2];

		vertex.Normal.x = currentVertex[3];
		vertex.Normal.y = currentVertex[4];
		vertex.Normal.z = currentVertex[5];

		vertex.TexCoords.x = currentVertex[6];
		vertex.TexCoords.y = currentVertex[7];

		vertices.push_back(vertex);

		indices.push_back(vertexId); // linear index system with GL_TRIANGLES
	}

	Mesh mesh(vertices, indices, textures);
	mesh.SetMode(GL_TRIANGLES);

	m_meshes.push_back(mesh);
}

//----------------------------------------------------------
void		Model::_ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(_ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		_ProcessNode(node->mChildren[i], scene);
	}
}

//----------------------------------------------------------
Mesh		Model::_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		// 
		//[...]

		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//[...]
	
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = _LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = _LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

//----------------------------------------------------------
std::vector<Texture>	Model::_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture>	textures;
	Game*					game = Game::Instance();
	RenderSystem*			rS = game->GetRenderSystem();

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;

		std::string textureStr = std::string(str.C_Str());
		std::string textureName = textureStr.substr(0, textureStr.find_last_of('.')); // gawd...
		std::string textureType = textureStr.substr(textureStr.find_last_of('.') + 1, textureStr.length());

		TextureResource* tex = rS->AddTextureResourceAndLoad(m_directory, textureName, textureType);

		texture.m_id = tex->TextureRenderId();
		texture.m_type = typeName;
		texture.m_path = str.C_Str();

		textures.push_back(texture);
	}
	return textures;
}

