#pragma once

#include "glew.h"
#include "assert.h"

#include <iostream>

typedef	unsigned int		uint;
typedef	unsigned __int64	u64;

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

#define	ARRAY_COUNT(__arr)				(sizeof(__arr) / sizeof((__arr)[0]))

#ifndef GLUE
#	define	GLUE(a, b)			__GLUE(a, b)
#	define	__GLUE(a, b)		a ## b			// don't directly use this one
#endif // !GLUE

#if	!defined(__COUNTER__)
#	define	__COUNTER__		__LINE__
#endif

#define MAX(a, b) a > b ? a : b;
#define MIN(a, b) a < b ? a : b;

template<typename _Type>
inline _Type	clamp(_Type x, _Type min, _Type max)
{
	return x > min ? x < max ? x : max : min;
}

template<typename _Type, typename _TypeFrac>
inline _Type	lerp(const _Type a, const _Type b, const _TypeFrac f)
{
	return a + f * (b - a);
}

#define PI 3.14159265359f

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

std::string		ImagePath(const std::string& fileName);
std::string		MeshPath(const std::string& fileName);
std::string		ShaderPath(const std::string& fileName);

std::string		ReadFromFile(const std::string& fileName);

// leak detection
//#include <vld.h>
