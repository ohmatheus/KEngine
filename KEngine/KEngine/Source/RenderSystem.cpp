#include "stdafx.h"

#include "RenderSystem.h"

#include "Game.h"
#include "GLShader.h"
#include "MeshData.h"

#include <sstream>
#include <map>

//----------------------------------------------------------
RenderSystem::RenderSystem(Game *game)
:	m_Game(game)
{
	_InitDefaultShader();
	_InitMeshDatas();
}

//----------------------------------------------------------
RenderSystem::~RenderSystem()
{
	for (std::map<std::string, GLShader*>::iterator it = m_ShaderBank.begin(); it != m_ShaderBank.end(); it++)
		delete it->second;
	m_ShaderBank.clear();

	for (std::map<std::string, MeshData*>::iterator it = m_MeshBank.begin(); it != m_MeshBank.end(); it++)
		delete it->second;
	m_MeshBank.clear();
}

//----------------------------------------------------------
void	RenderSystem::PreRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
bool	RenderSystem::RegisterShader(std::string const &shaderName, GLShader *shader)
{
	if (m_ShaderBank[shaderName] != nullptr)
		return false;
	m_ShaderBank[shaderName] = shader;
	return true;
}

//----------------------------------------------------------
bool	RenderSystem::RegisterMesh(std::string const &meshName, MeshData *mesh)
{
	if (m_MeshBank[meshName] != nullptr)
		return false;
	m_MeshBank[meshName] = mesh;
	return true;
}

//----------------------------------------------------------
void	RenderSystem::_InitDefaultShader()
{
	// DefaultShader
	{
		GLShader* shader = new GLShader();

		const char* vertexShader =
			"#version 410 core												\n"
			"layout(location = 0) in vec3 aPos;								\n"
			"uniform mat4 viewProj;											\n"
			"uniform mat4 model;											\n"
			"uniform mat4 view;												\n"
			"uniform mat4 proj;												\n"
			"void main()													\n"
			"{																\n"
			"	gl_Position = proj * view * model * vec4(aPos, 1.0f);		\n"
			"}																\n"
			;

		const char* fragShader =
			"#version 410 core												\n"
			"out vec4		FragColor;										\n"
			"uniform vec4	uColor;											\n"
			"void main()													\n"
			"{																\n"
			"	FragColor = uColor;											\n"
			"}																\n"
			;

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader);
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader);
		shader->CreateAndLink();
		shader->Bind();
		{
			shader->AddUniform("uColor");
			shader->AddUniform("model");
			shader->AddUniform("view");
			shader->AddUniform("proj");
		}
		shader->Unbind();

		GL_CHECK_ERRORS

		shader->m_name = "DefaultShader";
		RegisterShader("DefaultShader", shader);
	}

	// TexturedMeshShader
	{
		GLShader* shader = new GLShader();

		const char* vertexShader =
			"#version 410 core												\n"
			"layout(location = 0) in vec3 aPos;								\n"
			"layout(location = 1) in vec3 aColor;							\n"
			"layout(location = 2) in vec2 aTexCoord;						\n"
			"\n"
			"uniform mat4 model;											\n"
			"uniform mat4 view;												\n"
			"uniform mat4 proj;												\n"
			"\n"
			"out vec3 ourColor;												\n"
			"out vec2 TexCoord;												\n"
			"\n"
			"void main()													\n"
			"{																\n"
			"	gl_Position = proj * view * model * vec4(aPos, 1.0f);		\n"
			"	ourColor = aColor;											\n"
			"	TexCoord = aTexCoord;										\n"
			"}																\n"
			;

		const char* fragShader =
			"#version 410 core												\n"
			"\n"
			"in vec3		ourColor;										\n"
			"in vec2		TexCoord;										\n"
			"\n"
			"out vec4		FragColor;										\n"
			"\n"
			"uniform sampler2D	ourTexture;									\n"
			"\n"
			"void main()													\n"
			"{																\n"
			"	FragColor = texture(ourTexture, TexCoord);					\n"
			"}																\n"
			;

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader);
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader);
		shader->CreateAndLink();
		shader->Bind();
		{
			shader->AddUniform("model");
			shader->AddUniform("view");
			shader->AddUniform("proj");
		}
		shader->Unbind();

		GL_CHECK_ERRORS

		shader->m_name = "TexturedMeshShader";
		RegisterShader("TexturedMeshShader", shader);
	}
}

//----------------------------------------------------------
void	RenderSystem::_InitMeshDatas()
{
	{
		MeshData *mesh = new MeshData;
		float vertices[] =
		{
			-0.5f,	-0.5f,	0.f,
			0.5f,	-0.5f,	0.f,
			0.0f,	0.5f,	0.f
		};
		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLES);
		RegisterMesh("Triangle", mesh);
	}

	{
		MeshData *mesh = new MeshData;
		float vertices[] =
		{
			-0.5f,	-0.5f,	0.f,	// bot left 
			0.5f,	-0.5f,	0.f,	// bot right
			-0.5f,	0.5f,	0.f,	// top left
			0.5f,	0.5f,	0.f		// top right
		};
		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);
		RegisterMesh("Rectangle", mesh);
	}

	{
		MeshData *mesh = new MeshData;
		float vertices[] =
		{
			0.f,	-0.5f,	0.f,
			0.5f,	0.f,	0.f,
			-0.5f,	0.f,	0.f,
			0.f,	0.5f,	0.f
		};
		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);
		RegisterMesh("Diamond", mesh);
	}

	// ColoredRectangle
	{
		MeshData* mesh = new MeshData;
		float vertices[] = {
			// positions			// colors			// texture coords
			-0.5f,	-0.5f,	0.f,   1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// bot left 
			0.5f,	-0.5f,	0.f,   0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bot right
			-0.5f,	0.5f,	0.f,   0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		// top left
			0.5f,	0.5f,	0.f,   1.0f, 1.0f, 0.0f,	1.0f, 1.0f		// top right
		};
		//mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);

		//mesh->m_Size = ARRAY_COUNT(vertices);
		//mesh->m_Stride = stride;
		mesh->m_Mode = GL_TRIANGLE_STRIP;
		mesh->m_VerticeNbr = 4;

		glGenVertexArrays(1, &mesh->m_VAO);
		glGenBuffers(1, &mesh->m_VBO);

		glBindVertexArray(mesh->m_VAO);
		// bind, 1 possible in the same time / buffer type
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		RegisterMesh("ColoredTexturedRectangle", mesh);
	}
}
