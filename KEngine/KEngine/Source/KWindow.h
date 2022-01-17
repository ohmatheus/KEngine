#pragma once

#include <GLFW/glfw3.h>

//----------------------------------------------------------
class Game;

//----------------------------------------------------------
class KWindow
{
public:
	KWindow();
	virtual ~KWindow();

	void				CreateAndOpen(const char* name, float r, float g, float b);
	virtual void		ProcessWindowInput();
	bool				ShouldClose();
	void				SwapBuffers();
	virtual void		MakeCurrent();
	void				Clear();
	void				SetClearColor(float r, float g, float b);
	int					Width();
	int					Height();
	void				OnMouseMoved(float xpos, float ypos);

	void				SetGame(Game* game) { m_game = game; }
	Game*				GetGame() { return m_game; }

	GLFWwindow*			Window() { return m_window; }

private:
	Game*				m_game;
	GLFWwindow*			m_window;
	float				m_colors[3];
	glm::vec2			m_cursorPos;

};

//----------------------------------------------------------
