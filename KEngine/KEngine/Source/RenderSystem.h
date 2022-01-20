#pragma once

#include <map>
#include <memory>
#include <string>

//----------------------------------------------------------
class	Game;
class	GLShader;
class	MeshData;
class	Model;
class	TextureResource;
class	Scene;

//----------------------------------------------------------
class RenderSystem final
{
public:
	RenderSystem(Game *game);
	~RenderSystem();

	Game		*GetGame() { return m_game; }

	void		PreRender();

	bool				RegisterShader(std::string const &shaderName, GLShader *shader);
	bool				RegisterMesh(std::string const& meshName, MeshData* mesh);
	bool				RegisterTexture(std::string const& textureName, TextureResource* tex);
	bool				RegisterModel(std::string const &modelName, Model* model);

	GLShader*			GetShader(const std::string &name) { return m_ShaderBank[name]; }
	MeshData*			GetMesh(const std::string &name) { return m_MeshBank[name]; }
	TextureResource*	GetTexture(const std::string &name) { return m_TextureBank[name]; }

	void				LoadTexture(TextureResource *tex, const char* path = nullptr);
	TextureResource*	AddTextureResourceAndLoad(std::string const& directoryPath, std::string const &fileName, std::string const& fileType);

private:
	void	_LoadShaders();
	void	_LoadMeshes();
	void	_LoadTextures();
	void	_LoadModels();

	Game*									m_game;
	// scumbags resource managers
	std::map<std::string, GLShader*>		m_ShaderBank;
	std::map<std::string, MeshData*>		m_MeshBank;
	std::map<std::string, TextureResource*>	m_TextureBank;
	std::map<std::string, Model*>			m_modelBank;
};

//----------------------------------------------------------
