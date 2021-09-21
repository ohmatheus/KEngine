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
	void					Update();
	void					Render();

	void					TooglePlayStop();
	bool					IsPaused() { return m_Paused; }
	void					SetPaused(bool paused) { m_Paused = paused; }
	void					AddScene(IScene* scene);

	void					OnKeyEvent(int key, int scancode, int action, int mods);
	void					OnMouseMoved(double xpos, double ypos);
	void					OnMouseKeyEvent(int button, int action, int mods);
	void					OnScrollMoved(double xoffset, double yoffset);

	bool					IsGameRunning() { return m_IsGameRunning; }
	RenderSystem			*GetRenderSystem() { return m_RenderSystem; }
	IScene*					CurrentScene() { return m_currentScene; }


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
	RenderSystem			*m_RenderSystem;
	KWindow					*m_RenderWindow;
};

//----------------------------------------------------------
struct	SCamera
{
	float		m_Fov = 60.f;
	glm::vec3	m_Position = glm::vec3(0.f, 0.f, -2.f);
	glm::vec3	m_Rotation = glm::vec3(0.f);

	glm::mat4	m_ViewProj = glm::mat4(1.f);
	glm::mat4	m_OrthoMat;
	glm::mat4	m_ProjMat;

	glm::mat4	GetView()
	{
		glm::mat4 result = glm::mat4(1.f);

		result = glm::translate(result, m_Position);
		//result = glm::rotate(result, m_Rotation);
		return result;
	}
};

