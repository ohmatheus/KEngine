#include "stdafx.h"

#include "KWindow.h"
#include "GLShader.h"
#include "Game.h"

#include "GLFW\glfw3.h"

#include <iostream>

//----------------------------------------------------------
namespace
{
	int localWidth = 0;
	int localHeight = 0;

	//----------------------------------------------------------
	void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KWindow* gameWindow = (KWindow*)glfwGetWindowUserPointer(window);
		gameWindow->GetGame()->OnKeyEvent(key, scancode, action, mods);
	}

	//----------------------------------------------------------
	void onMouseMoved(GLFWwindow* window, double xpos, double ypos)
	{
		KWindow* gameWindow = (KWindow*)glfwGetWindowUserPointer(window);
		gameWindow->GetGame()->OnMouseMoved((float)xpos, (float)ypos);
	}

	//----------------------------------------------------------
	void onMouseKeyEvent(GLFWwindow* window, int button, int action, int mods)
	{
		KWindow* gameWindow = (KWindow*)glfwGetWindowUserPointer(window);
		gameWindow->GetGame()->OnMouseKeyEvent(button, action, mods);
	}

	//----------------------------------------------------------
	void onScrollMoved(GLFWwindow* window, double xoffset, double yoffset)
	{
		KWindow* gameWindow = (KWindow*)glfwGetWindowUserPointer(window);
		gameWindow->GetGame()->OnScrollMoved((float)xoffset, (float)yoffset);
	}
}

//----------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	localWidth = width;
	localHeight = height;
	glViewport(0, 0, width, height);
}

//----------------------------------------------------------
KWindow::KWindow()
:	m_window(nullptr)
,	m_game(nullptr)
{

}

//----------------------------------------------------------

KWindow::~KWindow()
{
}

//----------------------------------------------------------
void	KWindow::CreateAndOpen(const char* name, float r, float g, float b)
{
	SetClearColor(r, g, b);
	localWidth = 800;
	localHeight = 600;
	m_window = glfwCreateWindow(localWidth, localHeight, name, NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);

	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
	glViewport(0, 0, localWidth, localHeight);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	glfwSetKeyCallback(m_window, onKeyEvent);
	glfwSetCursorPosCallback(m_window, onMouseMoved);
	glfwSetMouseButtonCallback(m_window, onMouseKeyEvent);
	glfwSetScrollCallback(m_window, onScrollMoved);

	// maybe usefull :
	// {
	// 		glfwSetCursorEnterCallback(window, cursor_enter_callback);
	//		if (glfwGetWindowAttrib(window, GLFW_HOVERED))
	//		{
	//			highlight_interface();
	//		}
	// }
	
}

//----------------------------------------------------------
void	KWindow::ProcessWindowInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS)
	{
		int scanCode = glfwGetKeyScancode(GLFW_KEY_M);
		int scanCode2 = glfwGetKeyScancode(GLFW_KEY_N);
		void();
	}

	// propagate inputs
	// later change in internal events list
}

//----------------------------------------------------------
bool	KWindow::ShouldClose()
{
	return glfwWindowShouldClose(m_window);
}

//----------------------------------------------------------
void	KWindow::MakeCurrent()
{
	glfwMakeContextCurrent(m_window);
}

//----------------------------------------------------------
void	KWindow::Clear()
{
	glClearColor(m_colors[0], m_colors[1], m_colors[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void	KWindow::SwapBuffers()
{
	glfwMakeContextCurrent(m_window);
	glfwSwapBuffers(m_window);
	glfwMakeContextCurrent(m_window);
}

//----------------------------------------------------------
void		KWindow::SetClearColor(float r, float g, float b)
{
	m_colors[0] = r;
	m_colors[1] = g;
	m_colors[2] = b;
}

//----------------------------------------------------------
int			KWindow::Width()
{
	return localWidth;
}

//----------------------------------------------------------
int			KWindow::Height()
{
	return localHeight;
}

//----------------------------------------------------------
