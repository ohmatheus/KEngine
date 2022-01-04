#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <sstream>
#include <iostream>

//----------------------------------------------------------
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

//----------------------------------------------------------
std::string ImagePath(const std::string& fileName)
{
	std::string baseResourcePath("../External/_resources/images/");
	baseResourcePath += fileName;
	return baseResourcePath;
}

//----------------------------------------------------------
std::string MeshPath(const std::string& fileName)
{
	std::string baseResourcePath("../External/_resources/meshes/");
	baseResourcePath += fileName;
	return baseResourcePath;
}

//----------------------------------------------------------
std::string ShaderPath(const std::string& fileName)
{
	std::string baseResourcePath("../External/_resources/shaders/");
	baseResourcePath += fileName;
	return baseResourcePath;
}

//----------------------------------------------------------
std::string		ReadFromFile(const std::string& fileName)
{
	std::string		content;
	std::ifstream	fstream;
	// ensure ifstream objects can throw exceptions:
	fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		fstream.open(fileName);
		std::stringstream vStream;
		// read file's buffer contents into streams
		vStream << fstream.rdbuf();
		// close file handlers
		fstream.close();
		// convert stream into string
		content = vStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return content;
}


