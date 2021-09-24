#include "stdafx.h"

#include "BasicScene.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"

#include "GLShader.h"
#include "MeshData.h"

#include "stb_image.h"

//----------------------------------------------------------
BasicScene::BasicScene(Game* game)
:	IScene(game)
{}

//----------------------------------------------------------
BasicScene::~BasicScene()
{}

//----------------------------------------------------------
BasicScene::BasicScene(BasicScene const& other)
:	super(other)
{}

//----------------------------------------------------------
IScene* BasicScene::Clone()
{
	BasicScene* clone = new BasicScene(*this);
	return clone;
}

unsigned int g_textureContainer;
unsigned int g_textureFace;

//----------------------------------------------------------
void	BasicScene::BuildScene()
{
	m_ShaderName = "DefaultShader";
	m_MeshName = "Rectangle";
	m_Color = glm::vec4(1.f, 0.f, 1.f, 1.f);

	m_Position = glm::vec3(0.f);
	m_Rotation = glm::vec3(0.f);
	m_Scale = glm::vec3(1.f);

	m_Yaw = 25.f;
	m_Pitch = 0.f;
	m_Roll = 0.f;


	// Colored Rect
	{
		// -------- Texture --------
		glGenTextures(1, &g_textureContainer);
		glBindTexture(GL_TEXTURE_2D, g_textureContainer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		std::string path = ImagePath("container.jpg");
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << data << std::endl;
		}

		stbi_image_free(data);
	}

	// awesomeface.png
	{
		// -------- Texture --------
		glGenTextures(1, &g_textureFace);
		glBindTexture(GL_TEXTURE_2D, g_textureFace);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // GL_CLAMP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		std::string path = ImagePath("awesomeface.png");
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(false);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << data << std::endl;
		}

		stbi_image_free(data);
	}
	
}

//----------------------------------------------------------
void	BasicScene::OnSceneStart()
{}

//----------------------------------------------------------
void	BasicScene::Update(float dt)
{}

//----------------------------------------------------------
void	BasicScene::Render()
{
	{
		m_camera.m_OrthoMat = glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f);
		m_camera.m_ProjMat = glm::perspective(glm::radians(m_camera.m_Fov), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f);
		m_camera.m_ViewProj = m_camera.m_ProjMat * m_camera.GetView(); // what purpose ?
	}

	glm::mat4		modelW;
	{
		glm::mat4 result = glm::mat4(1.f);

		result = glm::translate(result, m_Position);
		result = glm::rotate(result, glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		result = glm::rotate(result, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		result = glm::rotate(result, glm::radians(m_Roll), glm::vec3(0.0f, 0.0f, 1.0f));
		result = glm::scale(result, m_Scale);

		modelW = result;

		//if (m_Parent != nullptr && worldspace)
		//	return m_Parent->ModelMatrix(worldspace) * result;
		//else
		//	return result;
	}

	// Basic
	if (false)
	{
		GLShader* shader = m_game->GetRenderSystem()->GetShader(m_ShaderName);
		MeshData* mesh = m_game->GetRenderSystem()->GetMesh(m_MeshName);
		
		assert(shader != nullptr);
		assert(mesh != nullptr);

		shader->Bind();
		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetView()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.m_ProjMat));
		glUniform4fv(shader->Uniform("uColor"), 1, glm::value_ptr(m_Color));
		glBindVertexArray(mesh->VAO());
		//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}

	// Textured rectangle
	if (false)
	{
		GLShader* shader = m_game->GetRenderSystem()->GetShader("TexturedMeshShader_Colored");
		MeshData* mesh = m_game->GetRenderSystem()->GetMesh("ColoredTexturedRectangle");

		assert(shader != nullptr);
		assert(mesh != nullptr);

		shader->Bind();
		
		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetView()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.m_ProjMat));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_textureContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_textureFace);
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture1"), 0); // set it manually
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture2"), 1); // set it manually


		glBindVertexArray(mesh->VAO());

		//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}

	// Textured cube
	if (true)
	{
		GLShader* shader = m_game->GetRenderSystem()->GetShader("TexturedMeshShader");
		MeshData* mesh = m_game->GetRenderSystem()->GetMesh("TexturedCube");

		assert(shader != nullptr);
		assert(mesh != nullptr);

		shader->Bind();

		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetView()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.m_ProjMat));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_textureContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_textureFace);
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture1"), 0); // set it manually
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture2"), 1); // set it manually


		glBindVertexArray(mesh->VAO());

		//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	m_camera.m_Position.z += yoffset;
}
