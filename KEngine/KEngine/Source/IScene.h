#pragma once

#include <vector>
#include <string>

#include "Camera.h"
//----------------------------------------------------------
class	RenderSystem;
class	Game;

//----------------------------------------------------------
class IScene
{
public:
	IScene(Game* game);
	virtual ~IScene();
	IScene(const IScene&);

	virtual IScene*			Clone() = 0; // could probably do this with crtp to automise with template but... no time
	virtual void			BuildScene() = 0;
	virtual void			OnSceneStart() = 0;
	virtual void			Update(float dt);
	virtual void			Render() = 0;

	virtual void			OnKeyEvent(int key, int scancode, int action, int mods);
	virtual void			OnMouseMoved(float xpos, float ypos);
	virtual void			OnMouseKeyEvent(int button, int action, int mods);
	virtual void			OnScrollMoved(float xoffset, float yoffset);

protected:
	Camera					m_camera;
	Game*					m_game;

};

//----------------------------------------------------------
