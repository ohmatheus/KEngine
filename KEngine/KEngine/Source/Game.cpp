#include "stdafx.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"

#include "Timer.h"

#include "IScene.h"
#include "Scenes/BasicScene.h"

#include "GLShader.h"

Game* Game::m_instance = nullptr;

//----------------------------------------------------------
Game::Game(KWindow *renderWindow)
:	m_RenderWindow(renderWindow)
,	m_RenderSystem(nullptr)
{
	assert(m_RenderWindow != nullptr);
	m_instance = this;
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
void		Game::Setup()
{
	int result = glewInit();
	if (result != GLEW_OK)
	{
		std::cout << "glewInit failed : " << glewGetErrorString(result);
		return;
	}

	m_RenderWindow->MakeCurrent();

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	_InitRenderSystem();

	m_Scenes.push_back(new BasicScene(this));

	m_currentScene = m_Scenes[0];

	m_currentScene->BuildScene();
}

//----------------------------------------------------------
void		Game::Update(float dt)
{
	m_currentScene->Update(dt);
}

//----------------------------------------------------------
void		Game::Render()
{
	m_RenderWindow->MakeCurrent();

	glViewport(0, 0, m_RenderWindow->Width(), m_RenderWindow->Height());

	m_currentScene->Render();
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
	m_currentScene->OnKeyEvent(key, scancode, action, mods);
}

//----------------------------------------------------------
void	Game::OnMouseMoved(float deltaX, float deltay)
{
	m_currentScene->OnMouseMoved(deltaX, deltay);
}

//----------------------------------------------------------
void	Game::OnMouseKeyEvent(int button, int action, int mods)
{
	m_currentScene->OnMouseKeyEvent(button, action, mods);
}

//----------------------------------------------------------
void	Game::OnScrollMoved(float xoffset, float yoffset)
{
	m_currentScene->OnScrollMoved(xoffset, yoffset);
}

//----------------------------------------------------------
void	Game::_InitRenderSystem()
{
	m_RenderSystem = new RenderSystem(this);
}

