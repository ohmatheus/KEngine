#include "stdafx.h"

#include "RenderSystem.h"

#include "Game.h"
#include "GLShader.h"
#include "MeshData.h"
#include "TextureResource.h"

#include <sstream>
#include <map>

#include "stb_image.h"

//----------------------------------------------------------
RenderSystem::RenderSystem(Game *game)
:	m_Game(game)
{
	_LoadShaders();
	_LoadMeshes();
	_LoadTextures();
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

	for (std::map<std::string, TextureResource*>::iterator it = m_TextureBank.begin(); it != m_TextureBank.end(); it++)
		delete it->second;
	m_TextureBank.clear();
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
bool	RenderSystem::RegisterTexture(std::string const& textureName, TextureResource* tex)
{
	if (m_TextureBank[textureName] != nullptr)
		return false;
	m_TextureBank[textureName] = tex;
	return true;
}

//----------------------------------------------------------
void	RenderSystem::LoadTexture(TextureResource* tex)
{
	glGenTextures(1, &tex->TextureRenderId());

	glBindTexture(GL_TEXTURE_2D, tex->TextureRenderId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->Attr().m_warpSMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->Attr().m_warpTMode); // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->Attr().m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->Attr().m_magFilter);

	int width, height, nrChannels;
	std::string path = ImagePath(tex->RelativePath());
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(false);
	if (data)
	{
		tex->Width() = width;
		tex->Height() = height;
		tex->NrChannels() = nrChannels;
		glTexImage2D(GL_TEXTURE_2D, 0, tex->Attr().m_internalFormat, width, height, 0, tex->Attr().m_fileFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << data << std::endl;
	}

	stbi_image_free(data);
}

//----------------------------------------------------------
void	RenderSystem::_LoadShaders()
{
	// DefaultShader
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("DefaultShader.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("DefaultShader.fshader"));

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader.c_str());
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader.c_str());
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

	// TexturedMeshShader colored
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("TexturedMeshShader_Colored.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("TexturedMeshShader_Colored.fshader"));

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader.c_str());
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader.c_str());
		shader->CreateAndLink();
		shader->Bind();
		{
			shader->AddUniform("model");
			shader->AddUniform("view");
			shader->AddUniform("proj");
		}
		shader->Unbind();

		GL_CHECK_ERRORS

		shader->m_name = "TexturedMeshShader_Colored";
		RegisterShader("TexturedMeshShader_Colored", shader);
	
	}

	// TexturedMeshShader
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("TexturedMeshShader.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("TexturedMeshShader.fshader"));

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader.c_str());
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader.c_str());
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

	// LightObject
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("LightObject.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("LightObject.fshader"));

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader.c_str());
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader.c_str());
		shader->CreateAndLink();
		shader->Bind();
		{
			shader->AddUniform("model");
			shader->AddUniform("view");
			shader->AddUniform("proj");
		}
		shader->Unbind();

		GL_CHECK_ERRORS

			shader->m_name = "LightObject";
		RegisterShader("LightObject", shader);
	}

	// SimpleLightCastedObject
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("SimpleLightCastedObject.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("SimpleLightCastedObject.fshader"));

		shader->LoadShader(GLShader::VERTEX_SHADER, vertexShader.c_str());
		shader->LoadShader(GLShader::FRAGMENT_SHADER, fragShader.c_str());
		shader->CreateAndLink();
		shader->Bind();
		{
			shader->AddUniform("model");
			shader->AddUniform("view");
			shader->AddUniform("proj");
			shader->AddUniform("viewPos");

			//shader->AddUniform("material.diffuse");
			//shader->AddUniform("material.specular");
			shader->AddUniform("material.shininess");

			shader->AddUniform("dirLight.direction");
			shader->AddUniform("dirLight.ambient");
			shader->AddUniform("dirLight.diffuse");
			shader->AddUniform("dirLight.specular");


			for (int i = 0; i < 4; i++)
			{
				std::string number = std::to_string(i);
				shader->AddUniform(("pointLights[" + number + "].position").c_str());
				shader->AddUniform(("pointLights[" + number + "].ambient").c_str());
				shader->AddUniform(("pointLights[" + number + "].diffuse").c_str());
				shader->AddUniform(("pointLights[" + number + "].specular").c_str());
				shader->AddUniform(("pointLights[" + number + "].constant").c_str());
				shader->AddUniform(("pointLights[" + number + "].linear").c_str());
				shader->AddUniform(("pointLights[" + number + "].quadratic").c_str());
			}

			for (int i = 0; i < 4; i++)
			{
				std::string number = std::to_string(i);
				shader->AddUniform(("spotLights[" + number + "].position").c_str());
				shader->AddUniform(("spotLights[" + number + "].direction").c_str());
				shader->AddUniform(("spotLights[" + number + "].ambient").c_str());
				shader->AddUniform(("spotLights[" + number + "].diffuse").c_str());
				shader->AddUniform(("spotLights[" + number + "].specular").c_str());
				shader->AddUniform(("spotLights[" + number + "].constant").c_str());
				shader->AddUniform(("spotLights[" + number + "].linear").c_str());
				shader->AddUniform(("spotLights[" + number + "].quadratic").c_str());
				shader->AddUniform(("spotLights[" + number + "].cutoff").c_str());
				shader->AddUniform(("spotLights[" + number + "].outerCutoff").c_str());
			}

		}
		shader->Unbind();

		GL_CHECK_ERRORS

		shader->m_name = "SimpleLightCastedObject";
		RegisterShader("SimpleLightCastedObject", shader);
	}
}

//----------------------------------------------------------
void	RenderSystem::_LoadMeshes()
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		RegisterMesh("ColoredTexturedRectangle", mesh);
	}

		// Cube Mesh
		float texturedCubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		float cubeNormalVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
			};

	// Simple Cube
	{
		MeshData* mesh = new MeshData;

		mesh->m_Mode = GL_TRIANGLES;
		mesh->m_VerticeNbr = 36;

		glGenVertexArrays(1, &mesh->m_VAO);
		glGenBuffers(1, &mesh->m_VBO);

		glBindVertexArray(mesh->m_VAO);
		// bind, 1 possible in the same time / buffer type
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(texturedCubeVertices) * sizeof(float), texturedCubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		RegisterMesh("SimpleCube", mesh);
	}

	// Textured Cube
	{
		MeshData* mesh = new MeshData;

		mesh->m_Mode = GL_TRIANGLES;
		mesh->m_VerticeNbr = 36;

		glGenVertexArrays(1, &mesh->m_VAO);
		glGenBuffers(1, &mesh->m_VBO);

		glBindVertexArray(mesh->m_VAO);
		// bind, 1 possible in the same time / buffer type
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(texturedCubeVertices) * sizeof(float), texturedCubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		RegisterMesh("TexturedCube", mesh);
	}

	// Cube + Normal + texture
	{
		MeshData* mesh = new MeshData;

		mesh->m_Mode = GL_TRIANGLES;
		mesh->m_VerticeNbr = 36;

		glGenVertexArrays(1, &mesh->m_VAO);
		glGenBuffers(1, &mesh->m_VBO);

		glBindVertexArray(mesh->m_VAO);
		// bind, 1 possible in the same time / buffer type
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(cubeNormalVertices) * sizeof(float), cubeNormalVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		RegisterMesh("SimpleCube_Normal", mesh);
	}
}

void	RenderSystem::_LoadTextures()
{
	{
		TextureResource*	tex = new TextureResource("container");
		TextureAttr			attr;
		attr.m_warpSMode = GL_REPEAT;
		attr.m_warpTMode = GL_REPEAT;
		attr.m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
		attr.m_magFilter = GL_LINEAR;
		attr.m_internalFormat = GL_RGB;
		attr.m_fileFormat = GL_RGB;
		tex->SetTextureAttributes(attr);
		tex->SetRelativePath("container.jpg");
		LoadTexture(tex);
		RegisterTexture(tex->Name(), tex);
	}

	{
		TextureResource*	tex = new TextureResource("smiley");
		TextureAttr			attr;
		attr.m_warpSMode = GL_CLAMP_TO_BORDER;
		attr.m_warpTMode = GL_CLAMP_TO_BORDER;
		attr.m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
		attr.m_magFilter = GL_LINEAR;
		attr.m_internalFormat = GL_RGB;
		attr.m_fileFormat = GL_RGBA;
		tex->SetTextureAttributes(attr);
		tex->SetRelativePath("awesomeface.png");
		LoadTexture(tex);
		RegisterTexture(tex->Name(), tex);
	}

	{
		TextureResource* tex = new TextureResource("container2");
		TextureAttr			attr;
		attr.m_warpSMode = GL_CLAMP_TO_BORDER;
		attr.m_warpTMode = GL_CLAMP_TO_BORDER;
		attr.m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
		attr.m_magFilter = GL_LINEAR;
		attr.m_internalFormat = GL_RGB;
		attr.m_fileFormat = GL_RGBA;
		tex->SetTextureAttributes(attr);
		tex->SetRelativePath("container2.png");
		LoadTexture(tex);
		RegisterTexture(tex->Name(), tex);
	}

	{
		TextureResource* tex = new TextureResource("container2_specularMap");
		TextureAttr			attr;
		attr.m_warpSMode = GL_CLAMP_TO_BORDER;
		attr.m_warpTMode = GL_CLAMP_TO_BORDER;
		attr.m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
		attr.m_magFilter = GL_LINEAR;
		attr.m_internalFormat = GL_RGB;
		attr.m_fileFormat = GL_RGBA;
		tex->SetTextureAttributes(attr);
		tex->SetRelativePath("container2_specular.png");
		LoadTexture(tex);
		RegisterTexture(tex->Name(), tex);
	}
}

