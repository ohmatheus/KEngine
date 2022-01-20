#pragma once

#include <vector>

//----------------------------------------------------------
class	IScene;
class	RenderSystem;
class	KWindow;
class	KApplication;

//----------------------------------------------------------
class Game final
{
public:
	Game(KWindow *renderWindow);
	~Game();

	void					Setup();
	void					Update(float dt);
	void					Render();

	void					TooglePlayStop();
	bool					IsPaused() { return m_paused; }
	void					SetPaused(bool paused) { m_paused = paused; }
	void					AddScene(IScene* scene);

	void					OnKeyEvent(int key, int scancode, int action, int mods);
	void					OnMouseMoved(float xpos, float ypos);
	void					OnMouseKeyEvent(int button, int action, int mods);
	void					OnScrollMoved(float xoffset, float yoffset);

	bool					IsGameRunning() { return m_isGameRunning; }
	RenderSystem*			GetRenderSystem() { return m_renderSystem; }
	KWindow*				GetRenderWindow() { return m_renderWindow; }
	IScene*					CurrentScene() { return m_currentScene; }

	static Game* Instance()
	{
		return m_instance;
	}

private:
	void					_InitRenderSystem();
	void					_StartScene();
	void					_StopScene();
	void					_CheckTogglePlayStop();

	float					m_Fps = 60.f;

	float					m_simulationSpeed = 1.f;
	bool					m_paused = false;
	bool					m_isGameRunning = false;
	bool					m_togglePlayStop = false;

	std::vector<IScene*>	m_scenes;
	IScene*					m_currentScene;
	RenderSystem*			m_renderSystem;
	KWindow*				m_renderWindow;

	static Game*			m_instance;
};

