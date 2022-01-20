#pragma once

#include <vector>

//----------------------------------------------------------
class GLShader;

//----------------------------------------------------------
enum class TextureType
{
	Diffuse,
	Specular
};

//----------------------------------------------------------
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//----------------------------------------------------------
struct Texture
{
	unsigned int	m_id;
	//TextureType		m_type;
	std::string		m_type;
	std::string		m_path;
};

//----------------------------------------------------------
class Mesh
{
public:
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;
	std::vector<Texture>		m_textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	void				Draw(GLShader* shader);

private:
	unsigned int m_VAO, m_VBO, m_EBO;
	uint						m_mode;

	void						_SetupMesh();
};