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
	unsigned int	m_Id;
	//TextureType		m_Type;
	std::string		m_Type;
	std::string		m_Path;
};

//----------------------------------------------------------
class Mesh
{
public:
	std::vector<Vertex>			m_Vertices;
	std::vector<unsigned int>	m_Indices;
	std::vector<Texture>		m_Textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	void				Draw(GLShader* shader);

private:
	unsigned int VAO, VBO, EBO;
	uint						m_Mode;

	void				SetupMesh();
};