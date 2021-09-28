#pragma once

#include "glew.h"

//----------------------------------------------------------
struct TextureAttr
{
	int				m_warpSMode = GL_REPEAT;
	int				m_warpTMode = GL_REPEAT;
	int				m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
	int				m_magFilter = GL_LINEAR;
	int				m_internalFormat = GL_RGB;
	int				m_fileFormat = GL_RGB;

};

//----------------------------------------------------------
class TextureResource
{
public:
	TextureResource(const std::string& name);
	~TextureResource();

	// reload file
	// update params/attributes

	const std::string&	Name() { return m_name; }

	unsigned int&		TextureRenderId() { return m_textureRenderId; }
	int&				Width() { return m_width; }
	int&				Height() { return m_height; }
	int&				NrChannels() { return m_nrChannels; }

	void				SetTextureAttributes(const TextureAttr &attr) { m_textureAttributes = attr; }
	void				SetRelativePath(const std::string& path) { m_relativePath = path; }
	const std::string&	RelativePath() { return m_relativePath; }
	const TextureAttr	&Attr() { return m_textureAttributes; }

private:
	unsigned int	m_textureRenderId = 0;
	int				m_width = 0;
	int				m_height = 0;
	int				m_nrChannels = 0;
	TextureAttr		m_textureAttributes;
	std::string		m_relativePath;
	std::string		m_name;
};