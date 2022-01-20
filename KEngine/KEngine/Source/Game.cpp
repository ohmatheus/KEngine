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
:	m_renderWindow(renderWindow)
,	m_renderSystem(nullptr)
{
	assert(m_renderWindow != nullptr);
	m_instance = this;
}

//----------------------------------------------------------
Game::~Game()
{
	for (size_t i = 0; i < m_scenes.size(); ++i)
		delete m_scenes[i];
	m_scenes.clear();

	if (m_isGameRunning)
		delete m_currentScene;

	if (m_renderSystem)
		delete m_renderSystem;
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

	m_renderWindow->MakeCurrent();

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	_InitRenderSystem();

	m_scenes.push_back(new BasicScene(this));

	m_currentScene = m_scenes[0];

	m_currentScene->BuildScene();
}

//----------------------------------------------------------
void		Game::Update(float dt)
{
	m_currentScene->Update(m_paused ? 0.f : dt);
}

//----------------------------------------------------------
void		Game::Render()
{
	m_renderWindow->MakeCurrent();

	glViewport(0, 0, m_renderWindow->Width(), m_renderWindow->Height());

	m_currentScene->Render();
}

//----------------------------------------------------------
void	Game::TooglePlayStop()
{
	m_togglePlayStop = true;
}

//----------------------------------------------------------
void	Game::_StartScene()
{
	m_currentScene = m_scenes[0]->Clone();
	m_currentScene->OnSceneStart();
	m_isGameRunning = true;
	m_paused = false;
}

//----------------------------------------------------------
void	Game::_StopScene()
{
	if (m_currentScene != nullptr)
	{
		delete m_currentScene;
	}
	m_currentScene = m_scenes[0];
	m_isGameRunning = false;
	m_paused = false;
}

//----------------------------------------------------------
void	Game::_CheckTogglePlayStop()
{
	if (m_togglePlayStop)
	{
		if (!m_isGameRunning)
			_StartScene();
		else
			_StopScene();
		m_togglePlayStop = false;
	}
}

//----------------------------------------------------------
void	Game::AddScene(IScene* scene)
{
	m_scenes.push_back(scene);
	m_currentScene = scene;
}

//----------------------------------------------------------
void	Game::OnKeyEvent(int key, int scancode, int action, int mods)
{
	m_currentScene->OnKeyEvent(key, scancode, action, mods);
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		m_paused = !m_paused;
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
	m_renderSystem = new RenderSystem(this);
	m_renderSystem->LoadAll();
}

