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
	bool					IsPaused() { return m_Paused; }
	void					SetPaused(bool paused) { m_Paused = paused; }
	void					AddScene(IScene* scene);

	void					OnKeyEvent(int key, int scancode, int action, int mods);
	void					OnMouseMoved(float xpos, float ypos);
	void					OnMouseKeyEvent(int button, int action, int mods);
	void					OnScrollMoved(float xoffset, float yoffset);

	bool					IsGameRunning() { return m_IsGameRunning; }
	RenderSystem*			GetRenderSystem() { return m_RenderSystem; }
	KWindow*				GetRenderWindow() { return m_RenderWindow; }
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

	float					m_SimulationSpeed = 1.f;
	bool					m_Paused = false;
	bool					m_IsGameRunning = false;
	bool					m_TogglePlayStop = false;

	std::vector<IScene*>	m_Scenes;
	IScene*					m_currentScene;
	RenderSystem*			m_RenderSystem;
	KWindow*				m_RenderWindow;

	static Game*			m_instance;
};

