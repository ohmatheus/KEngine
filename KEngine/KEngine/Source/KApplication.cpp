#include "stdafx.h"
#include "KApplication.h"

#include "KWindow.h"

#include "Game.h"
#include "Timer.h"

#include <GLFW/glfw3.h>


//----------------------------------------------------------
KApplication::KApplication()
:	m_RenderWindow(nullptr)
,	m_Game(nullptr)
{}

//----------------------------------------------------------
KApplication::~KApplication()
{
	if (m_Game != nullptr)
		delete m_Game;
	if (m_RenderWindow)
		delete m_RenderWindow;
	if (m_ControllerWindow)
		delete m_ControllerWindow;
}

//----------------------------------------------------------
void	KApplication::Setup()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_ControllerWindow = new KWindow();
	m_ControllerWindow->CreateAndOpen("K Controller Window", 0.3f, 0.2f, 0.3f);

	m_RenderWindow = new KWindow();
	m_RenderWindow->CreateAndOpen("K Main Window", 0.2f, 0.3f, 0.3f);

	//glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);

	m_Game = new Game(m_RenderWindow);
	m_RenderWindow->SetGame(m_Game);
	m_ControllerWindow->SetGame(m_Game);
	m_Game->Setup();
}

//----------------------------------------------------------
void	KApplication::Loop()
{
	Timer timer;
	double			dt = 0.0;

	const double	fixedSimulationFramerate = 60.0;
	double			limitedDt = 0;
	timer.Start();

	while (!m_RenderWindow->ShouldClose() && !m_ControllerWindow->ShouldClose())
	{
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->Clear();

		m_RenderWindow->MakeCurrent();
		m_RenderWindow->Clear();

		glfwPollEvents(); // input callbacks are unstacked here

		// input
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->ProcessWindowInput();
		m_RenderWindow->MakeCurrent();
		m_RenderWindow->ProcessWindowInput();

		// logic
		m_Game->Update((float)dt);
		m_Game->Render();

		// rendering

		// check and call events and swap buffers
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->SwapBuffers();
		m_RenderWindow->MakeCurrent();
		m_RenderWindow->SwapBuffers();

		//glfwWaitEventsTimeout(0.7); seems interesting for editor side

		dt = timer.Stop();
		timer.Start();
		// fix framerate (not very precise tho... but good enough)
		{
			const double	dtLimit = 1.0f / fixedSimulationFramerate;
			const double	timeDelta = dtLimit - dt;
			const double	timeToWait = (timeDelta * 1.0e+3f) - 1.0f;

			if (dt > dtLimit)
				dt = dtLimit;

			//if (timeToWait > 0.f)
			//	QThread::msleep(uint(timeToWait));
		}
		//std::cout << "tick" << std::endl;
	}
}

//----------------------------------------------------------
void	KApplication::Terminate()
{
	glfwTerminate();
}

//----------------------------------------------------------

