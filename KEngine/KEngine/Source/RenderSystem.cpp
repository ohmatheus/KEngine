#include "stdafx.h"

#include "RenderSystem.h"

#include "Game.h"
#include "GLShader.h"
#include "TextureResource.h"
#include "Model.h"

#include <sstream>
#include <map>

#include "stb_image.h"

//----------------------------------------------------------
RenderSystem::RenderSystem(Game *game)
:	m_game(game)
{

}

//----------------------------------------------------------
void	RenderSystem::LoadAll()
{
	_LoadShaders();
	_LoadTextures();
	_LoadModels();
}

//----------------------------------------------------------
RenderSystem::~RenderSystem()
{
	for (std::map<std::string, GLShader*>::iterator it = m_shaderBank.begin(); it != m_shaderBank.end(); it++)
		delete it->second;
	m_shaderBank.clear();

	for (std::map<std::string, Model*>::iterator it = m_modelBank.begin(); it != m_modelBank.end(); it++)
		delete it->second;
	m_modelBank.clear();

	for (std::map<std::string, TextureResource*>::iterator it = m_textureBank.begin(); it != m_textureBank.end(); it++)
		delete it->second;
	m_textureBank.clear();
}

//----------------------------------------------------------
void	RenderSystem::PreRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
bool	RenderSystem::RegisterShader(std::string const &shaderName, GLShader *shader)
{
	if (m_shaderBank[shaderName] != nullptr)
		return false;
	m_shaderBank[shaderName] = shader;
	return true;
}

//----------------------------------------------------------
bool	RenderSystem::RegisterTexture(std::string const& textureName, TextureResource* tex)
{
	if (m_textureBank[textureName] != nullptr)
		return false;
	m_textureBank[textureName] = tex;
	return true;
}

//----------------------------------------------------------
bool	RenderSystem::RegisterModel(std::string const& modelName, Model* model)
{
	if (m_modelBank[modelName] != nullptr)
		return false;
	m_modelBank[modelName] = model;
	return true;
}

//----------------------------------------------------------
void	RenderSystem::LoadTexture(TextureResource* tex, const char* filePath)
{
	glGenTextures(1, &tex->TextureRenderId());

	glBindTexture(GL_TEXTURE_2D, tex->TextureRenderId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->Attr().m_warpSMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->Attr().m_warpTMode); // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->Attr().m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->Attr().m_magFilter);

	int width, height, nrChannels;
	std::string path;
	if (filePath == nullptr)
		path = ImagePath(tex->RelativePath());
	else
	{
		path = filePath + std::string("/") + tex->RelativePath();
	}
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
TextureResource* RenderSystem::AddTextureResourceAndLoad(std::string const& directoryPath, std::string const& fileName, std::string const& fileType)
{
	if (m_textureBank[fileName] != nullptr)
		return m_textureBank[fileName];

	TextureResource* tex = new TextureResource(fileName);
	TextureAttr			attr;
	attr.m_warpSMode = GL_CLAMP_TO_BORDER;
	attr.m_warpTMode = GL_CLAMP_TO_BORDER;
	attr.m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
	attr.m_magFilter = GL_LINEAR;
	attr.m_internalFormat = GL_RGB;
	attr.m_fileFormat = GL_RGB;// GL_RGBA;
	tex->SetTextureAttributes(attr);
	tex->SetRelativePath(fileName + "." + fileType);
	LoadTexture(tex, directoryPath.c_str());
	RegisterTexture(tex->Name(), tex);

	return tex;
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

	// SimpleModel
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("SimpleModel.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("SimpleModel.fshader"));

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

		shader->m_name = "SimpleModel";
		RegisterShader("SimpleModel", shader);
	}

	// Depth Shader
	{
		GLShader* shader = new GLShader();

		std::string vertexShader = ReadFromFile(ShaderPath("Depth.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("Depth.fshader"));

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

		shader->m_name = "Depth";
		RegisterShader("Depth", shader);
	}

	// Outline Shader
	{
		GLShader* shader = new GLShader();
	
		std::string vertexShader = ReadFromFile(ShaderPath("OutlineShader.vshader"));
		std::string fragShader = ReadFromFile(ShaderPath("OutlineShader.fshader"));
	
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
	
		shader->m_name = "OutlineShader";
		RegisterShader("OutlineShader", shader);
	}
}

const float cubeNormalVertices[] = {
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
//
////----------------------------------------------------------
//void	RenderSystem::_LoadMeshes()
//{
//	{
//		MeshData *mesh = new MeshData;
//		float vertices[] =
//		{
//			-0.5f,	-0.5f,	0.f,
//			0.5f,	-0.5f,	0.f,
//			0.0f,	0.5f,	0.f
//		};
//		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLES);
//		RegisterMesh("Triangle", mesh);
//	}
//
//	{
//		MeshData *mesh = new MeshData;
//		float vertices[] =
//		{
//			-0.5f,	-0.5f,	0.f,	// bot left 
//			0.5f,	-0.5f,	0.f,	// bot right
//			-0.5f,	0.5f,	0.f,	// top left
//			0.5f,	0.5f,	0.f		// top right
//		};
//		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);
//		RegisterMesh("Rectangle", mesh);
//	}
//
//	{
//		MeshData *mesh = new MeshData;
//		float vertices[] =
//		{
//			0.f,	-0.5f,	0.f,
//			0.5f,	0.f,	0.f,
//			-0.5f,	0.f,	0.f,
//			0.f,	0.5f,	0.f
//		};
//		mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);
//		RegisterMesh("Diamond", mesh);
//	}
//
//	// ColoredRectangle
//	{
//		MeshData* mesh = new MeshData;
//		float vertices[] = {
//			// positions			// colors			// texture coords
//			-0.5f,	-0.5f,	0.f,   1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// bot left 
//			0.5f,	-0.5f,	0.f,   0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bot right
//			-0.5f,	0.5f,	0.f,   0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		// top left
//			0.5f,	0.5f,	0.f,   1.0f, 1.0f, 0.0f,	1.0f, 1.0f		// top right
//		};
//		//mesh->SetVertices(vertices, ARRAY_COUNT(vertices), 3, 3 * sizeof(float), GL_TRIANGLE_STRIP);
//
//		//mesh->m_size = ARRAY_COUNT(vertices);
//		//mesh->m_stride = stride;
//		mesh->m_mode = GL_TRIANGLE_STRIP;
//		mesh->m_verticeNbr = 4;
//
//		glGenVertexArrays(1, &mesh->m_VAO);
//		glGenBuffers(1, &mesh->m_VBO);
//
//		glBindVertexArray(mesh->m_VAO);
//		// bind, 1 possible in the same time / buffer type
//		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
//		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//		glEnableVertexAttribArray(2);
//
//		glEnableVertexAttribArray(0);
//		glBindVertexArray(0);
//
//		RegisterMesh("ColoredTexturedRectangle", mesh);
//	}
//
//		// Cube Mesh
//		float texturedCubeVertices[] = {
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//		};
//
//	// Simple Cube
//	{
//		MeshData* mesh = new MeshData;
//
//		mesh->m_mode = GL_TRIANGLES;
//		mesh->m_verticeNbr = 36;
//
//		glGenVertexArrays(1, &mesh->m_VAO);
//		glGenBuffers(1, &mesh->m_VBO);
//
//		glBindVertexArray(mesh->m_VAO);
//		// bind, 1 possible in the same time / buffer type
//		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);
//		glBufferData(GL_ARRAY_BUFFER, ARRAY_COUNT(texturedCubeVertices) * sizeof(float), texturedCubeVertices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(0);
//
//		glEnableVertexAttribArray(0);
//		glBindVertexArray(0);
//
//		RegisterMesh("SimpleCube", mesh);
//	}
//
//}

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

void	RenderSystem::_LoadModels()
{
	{
		Model* model = new Model();
		model->LoadModel(ModelPath("backpack/backpack.obj").c_str());
		RegisterModel("backpack", model);
	}

	{
		Model* model = new Model();
		std::vector<Texture> textures;
		SVerticesArrayProperties props;

		props.m_vertices = cubeNormalVertices;
		props.m_nRVertices = 36;

		Texture texDiffuse;
		texDiffuse.m_type = "texture_diffuse";
		texDiffuse.m_id = m_textureBank["container2"]->TextureRenderId();
		textures.push_back(texDiffuse);

		Texture texSpec;
		texSpec.m_type = "texture_specular";
		texSpec.m_id = m_textureBank["container2_specularMap"]->TextureRenderId();
		textures.push_back(texSpec);

		model->LoadMeshFromVertexArray(props, textures);
		RegisterModel("ContainerCube", model);
	}

	{
		Model* model = new Model();
		std::vector<Texture> textures;
		SVerticesArrayProperties props;

		props.m_vertices = cubeNormalVertices;
		props.m_nRVertices = 36;

		model->LoadMeshFromVertexArray(props, textures);
		RegisterModel("SimpleCube", model);
	}
}


