#include "stdafx.h"

#include "Game.h"

#include "RenderSystem.h"
#include "KWindow.h"
#include "Timer.h"
#include "IScene.h"

#include "MeshData.h"
#include "GLShader.h"

//----------------------------------------------------------
Game::Game(KWindow *renderWindow)
:	m_RenderWindow(renderWindow)
,	m_RenderSystem(nullptr)
{
	assert(m_RenderWindow != nullptr);
}

//----------------------------------------------------------
Game::~Game()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
		delete m_Scenes[i];
	m_Scenes.clear();

	if (m_IsGameRunning)
		delete m_currentScene;

	if (m_RenderSystem)
		delete m_RenderSystem;
}

//----------------------------------------------------------
glm::vec3				g_Position;
glm::vec3				g_Rotation;
glm::vec3				g_Scale;
glm::vec4				g_Color;

float					g_Pitch;
float					g_Yaw;
float					g_Roll;

std::string				g_ShaderName; // TODO replace by an ID
std::string				g_MeshName; // TODO replace by an ID

SCamera					g_camera;

//----------------------------------------------------------
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

//----------------------------------------------------------
void		Game::Setup()
{
	int result = glewInit();
	if (result != GLEW_OK)
	{
		std::cout << "glewInit failed : " << glewGetErrorString(result);
		return;
	}

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	m_RenderWindow->MakeCurrent();

	_InitRenderSystem();

	{
		g_ShaderName = "DefaultShader";
		g_MeshName = "Rectangle";
		g_Color = glm::vec4(1.f, 0.f, 1.f, 1.f);

		g_Position = glm::vec3(0.f);
		g_Rotation = glm::vec3(0.f);
		g_Scale = glm::vec3(1.f);

		g_Yaw = 0.f;
		g_Pitch = 0.f;
		g_Roll = 0.f;
	}
}

//----------------------------------------------------------
void		Game::Update()
{

}

//----------------------------------------------------------
void		Game::Render()
{
	m_RenderWindow->MakeCurrent();

	glViewport(0, 0, m_RenderWindow->Width(), m_RenderWindow->Height());

	{
		g_camera.m_OrthoMat = glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f);
		g_camera.m_ProjMat = glm::perspective(glm::radians(g_camera.m_Fov), (float)m_RenderWindow->Width() / (float)m_RenderWindow->Height(), 0.1f, 200.0f);
	
		g_camera.m_ViewProj = g_camera.m_ProjMat * g_camera.GetView(); // what purpose ?
	}

	{
		GLShader* shader = m_RenderSystem->GetShader(g_ShaderName);
		MeshData* mesh = m_RenderSystem->GetMesh(g_MeshName);
		IScene* currentScene = m_RenderSystem->GetGame()->CurrentScene();

		glm::mat4		modelW;
		{
			glm::mat4 result = glm::mat4(1.f);

			result = glm::translate(result, g_Position);
			result = glm::rotate(result, glm::radians(g_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			result = glm::rotate(result, glm::radians(g_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			result = glm::rotate(result, glm::radians(g_Roll), glm::vec3(0.0f, 0.0f, 1.0f));
			result = glm::scale(result, g_Scale);

			modelW = result;

			//if (m_Parent != nullptr && worldspace)
			//	return m_Parent->ModelMatrix(worldspace) * result;
			//else
			//	return result;
		}
		assert(shader != nullptr);
		assert(mesh != nullptr);

		shader->Bind();
		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(g_camera.GetView()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(g_camera.m_ProjMat));
		glUniform4fv(shader->Uniform("uColor"), 1, glm::value_ptr(g_Color));
		glBindVertexArray(mesh->VAO());
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO());
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}
}

//----------------------------------------------------------
void	Game::TooglePlayStop()
{
	m_TogglePlayStop = true;
}

//----------------------------------------------------------
void	Game::_StartScene()
{
	m_currentScene = m_Scenes[0]->Clone();
	m_currentScene->OnSceneStart();
	m_IsGameRunning = true;
	m_Paused = false;
}

//----------------------------------------------------------
void	Game::_StopScene()
{
	if (m_currentScene != nullptr)
	{
		delete m_currentScene;
	}
	m_currentScene = m_Scenes[0];
	m_IsGameRunning = false;
	m_Paused = false;
}

//----------------------------------------------------------
void	Game::_CheckTogglePlayStop()
{
	//SCOPEDLOCK(m_UILock);
	if (m_TogglePlayStop)
	{
		if (!m_IsGameRunning)
			_StartScene();
		else
			_StopScene();
		m_TogglePlayStop = false;
	}
}

//----------------------------------------------------------
void	Game::AddScene(IScene* scene)
{
	m_Scenes.push_back(scene);
	m_currentScene = scene;
}

//----------------------------------------------------------
void	Game::OnKeyEvent(int key, int scancode, int action, int mods)
{

}

//----------------------------------------------------------
void	Game::OnMouseMoved(double xpos, double ypos)
{

}

//----------------------------------------------------------
void	Game::OnMouseKeyEvent(int button, int action, int mods)
{

}

//----------------------------------------------------------
void	Game::OnScrollMoved(double xoffset, double yoffset)
{
	g_camera.m_Position.z += yoffset;
}

//----------------------------------------------------------
void	Game::_InitRenderSystem()
{
	m_RenderSystem = new RenderSystem(this);
}

