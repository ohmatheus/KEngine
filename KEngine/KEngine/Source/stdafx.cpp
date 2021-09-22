#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

