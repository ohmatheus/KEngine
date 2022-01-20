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

	Game*				GetGame() { return m_game; }

	void				PreRender();
	void				LoadAll();

	bool				RegisterShader(std::string const &shaderName, GLShader *shader);
	bool				RegisterTexture(std::string const& textureName, TextureResource* tex);
	bool				RegisterModel(std::string const &modelName, Model* model);

	GLShader*			GetShader(const std::string &name) { return m_shaderBank[name]; }
	TextureResource*	GetTexture(const std::string &name) { return m_textureBank[name]; }
	Model*				GetModel(const std::string &name) { return m_modelBank[name]; }

	void				LoadTexture(TextureResource *tex, const char* path = nullptr);
	TextureResource*	AddTextureResourceAndLoad(std::string const& directoryPath, std::string const &fileName, std::string const& fileType);

private:
	void	_LoadShaders();
	void	_LoadTextures();
	void	_LoadModels();

	Game*									m_game;

	// scumbag resource managers
	std::map<std::string, GLShader*>		m_shaderBank;
	std::map<std::string, TextureResource*>	m_textureBank;
	std::map<std::string, Model*>			m_modelBank;
};

//----------------------------------------------------------
