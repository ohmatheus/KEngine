#include "stdafx.h"

#include "GLShader.h"
#include <iostream>

//----------------------------------------------------
GLShader::GLShader()
{
	m_Shaders[FRAGMENT_SHADER] = 0u;
	m_Shaders[GEOMETRY_SHADER] = 0u;
	m_Shaders[VERTEX_SHADER] = 0u;
	m_ProgramID = 0u;
}

//----------------------------------------------------
GLShader::~GLShader()
{
	m_Attributes.clear();
	m_UniformsLocation.clear();
}

//----------------------------------------------------
bool	GLShader::LoadShader(uint type, const char *shader)
{
	uint  glType;
	switch (type)
	{
	case VERTEX_SHADER: glType = GL_VERTEX_SHADER; break;
	case FRAGMENT_SHADER: glType = GL_FRAGMENT_SHADER; break;
	case GEOMETRY_SHADER: glType = GL_GEOMETRY_SHADER; break;
	default: return false;
	}
	m_Shaders[type] = _loadShader(glType, shader);
	GL_CHECK_ERRORS

	return (m_Shaders[type] != 0);
}

//----------------------------------------------------
bool	GLShader::CreateAndLink()
{
	m_ProgramID = glCreateProgram();
	if (m_ProgramID == 0)
	{
		std::cout << "Failed to create shader program.";
		return false;
	}
	if (m_Shaders[VERTEX_SHADER])
		glAttachShader(m_ProgramID, m_Shaders[VERTEX_SHADER]);
	if (m_Shaders[FRAGMENT_SHADER])
		glAttachShader(m_ProgramID, m_Shaders[FRAGMENT_SHADER]);
	glLinkProgram(m_ProgramID);
	GLint linked = 0;
	GLint infoLen = 0;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(m_ProgramID, infoLen, nullptr, infoLog);
			std::string str(infoLog);
			std::cout << "link error: " << str;
			free(infoLog);
		}
		glDeleteProgram(m_ProgramID);
		return false;
	}
	glValidateProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLen);
	if (infoLen > 1)
	{
		char* infoLog = (char*)malloc(sizeof(char) * infoLen);
		glGetProgramInfoLog(m_ProgramID, infoLen, nullptr, infoLog);
		std::string str(infoLog);

		std::cout << "Shader program validation results: " + str;
		free(infoLog);
	}
	glDeleteShader(m_Shaders[VERTEX_SHADER]);
	glDeleteShader(m_Shaders[FRAGMENT_SHADER]);
	return true;
}

//----------------------------------------------------
void GLShader::Destroy()
{
	for (uint i = 0; i < ARRAY_COUNT(m_Shaders); i++)
	{
		if (m_Shaders[i] != 0u)
		{
			glDetachShader(m_ProgramID, m_Shaders[i]);
			//glDeleteShader(m_Shaders[i]);
		}
	}
	if (m_ProgramID != 0u)
		glDeleteProgram(m_ProgramID);
}

//----------------------------------------------------
unsigned int GLShader::Bind()
{
	glUseProgram(m_ProgramID);
	return m_ProgramID;
}

//----------------------------------------------------
void GLShader::Unbind()
{
	glUseProgram(0);
}

//----------------------------------------------------
void GLShader::AddAttribute(std::string const& attribute)
{
	m_Attributes[attribute] = glGetAttribLocation(m_ProgramID, attribute.c_str());
}

//----------------------------------------------------
uint GLShader::Attribute(std::string const& attribute)
{
	uint	result = m_Attributes[attribute];
	assert(result != -1);
	return result;
}

//----------------------------------------------------
void GLShader::AddUniform(std::string const& uniform)
{
	uint uniformLocation = glGetUniformLocation(m_ProgramID, uniform.c_str());
	m_UniformsLocation[uniform] = uniformLocation;
}

//----------------------------------------------------
uint GLShader::Uniform(std::string const& uniform)
{
	uint	result = m_UniformsLocation[uniform];
	assert(result != -1);
	return result;
}

//----------------------------------------------------
uint GLShader::_getProgramID() const
{
	return m_ProgramID;
}

//----------------------------------------------------
uint GLShader::_loadShader(GLenum type, std::string const& source)
{
	//char* text = TempUtils::Instance()->FileToChar(source);
	assert(false);
	//if (text == nullptr)
	//	return 0;

	return LoadShader(type, nullptr);
}

//----------------------------------------------------
uint GLShader::_loadShader(uint type, const char *text)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
		return 0;
	glShaderSource(shader, 1, (const char**)&text, nullptr);
	//free(text);
	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
			std::string str(infoLog);
			std::cout << "Error while compiling shader: " << str;
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

//----------------------------------------------------
