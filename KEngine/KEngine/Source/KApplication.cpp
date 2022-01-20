#include "stdafx.h"
#include "KApplication.h"

#include "KWindow.h"

#include "Game.h"
#include "Timer.h"

#include <GLFW/glfw3.h>


//----------------------------------------------------------
KApplication::KApplication()
:	m_renderWindow(nullptr)
,	m_game(nullptr)
{}

//----------------------------------------------------------
KApplication::~KApplication()
{
	if (m_game != nullptr)
		delete m_game;
	if (m_renderWindow)
		delete m_renderWindow;
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

	m_renderWindow = new KWindow();
	m_renderWindow->CreateAndOpen("K Main Window", 0.2f, 0.3f, 0.3f);

	//glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);

	m_game = new Game(m_renderWindow);
	m_renderWindow->SetGame(m_game);
	m_ControllerWindow->SetGame(m_game);
	m_game->Setup();
}

//----------------------------------------------------------
void	KApplication::Loop()
{
	Timer timer;
	double			dt = 0.0;

	const double	fixedSimulationFramerate = 60.0;
	double			limitedDt = 0;
	timer.Start();

	while (!m_renderWindow->ShouldClose() && !m_ControllerWindow->ShouldClose())
	{
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->Clear();

		m_renderWindow->MakeCurrent();
		m_renderWindow->Clear();

		glfwPollEvents(); // input callbacks are unstacked here

		// input
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->ProcessWindowInput();
		m_renderWindow->MakeCurrent();
		m_renderWindow->ProcessWindowInput();

		// logic
		m_game->Update((float)dt);
		m_game->Render();

		// rendering

		// check and call events and swap buffers
		m_ControllerWindow->MakeCurrent();
		m_ControllerWindow->SwapBuffers();
		m_renderWindow->MakeCurrent();
		m_renderWindow->SwapBuffers();

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

