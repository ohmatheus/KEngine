#pragma once

#include <map>
#include <memory>
#include <string>

//----------------------------------------------------------
class	Game;
class	GLShader;
class	MeshData;
class	Scene;

//----------------------------------------------------------
class RenderSystem final
{
public:
	RenderSystem(Game *game);
	~RenderSystem();

	Game		*GetGame() { return m_Game; }

	void		PreRender();

	bool		RegisterShader(std::string const &shaderName, GLShader *shader);
	bool		RegisterMesh(std::string const &meshName, MeshData *mesh);

	GLShader	*GetShader(const std::string &name) { return m_ShaderBank[name]; }
	MeshData	*GetMesh(const std::string &name) { return m_MeshBank[name]; }

private:
	void	_InitDefaultShader();
	void	_InitMeshDatas();

	Game								*m_Game;
	// scumbags resource managers
	std::map<std::string, GLShader*>	m_ShaderBank;
	std::map<std::string, MeshData*>	m_MeshBank;
};

//----------------------------------------------------------
