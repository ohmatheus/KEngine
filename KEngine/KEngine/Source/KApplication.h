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
	KWindow				*m_RenderWindow;
	KWindow				*m_ControllerWindow;
	Game				*m_Game;
};

