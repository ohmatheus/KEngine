#pragma once

class	Game;
class	KWindow;

//----------------------------------------------------------
class KApplication
{
public:
	KApplication();
	virtual ~KApplication();

	void				Setup();
	void				Loop();
	void				Terminate();

private:
	KWindow				*m_renderWindow;
	KWindow				*m_ControllerWindow;
	Game				*m_game;
};

