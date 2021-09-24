#pragma once

#include <string>
#include <map>

//----------------------------------------------------------
class GLShader final
{
public:
	GLShader();
	~GLShader();

	bool			LoadShader(uint, const char *shader);

	bool			CreateAndLink();
	void			Destroy();

	uint			Bind();
	void			Unbind();

	void			AddAttribute(const std::string& attribute);
	void			AddUniform(const std::string& unform);

	uint			Attribute(const std::string& attribute);
	uint			Uniform(const std::string& uniform);

	uint			ProgramID() { return m_ProgramID; }

	enum	ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		GEOMETRY_SHADER
	};
	std::string						m_name;

private:
	uint							m_ProgramID;
	uint							m_Shaders[3];
	std::map<std::string, uint>		m_Attributes;
	std::map<std::string, uint>		m_UniformsLocation;

private:
	uint			_getProgramID() const;
	unsigned int	_loadShader(uint type, const std::string &fileName);
	unsigned int	_loadShader(uint type, const char *shader);
};

//----------------------------------------------------------
