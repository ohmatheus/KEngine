#pragma once

#include <vector>
#include <string>

//----------------------------------------------------------
class	RenderSystem;
class	Game;

//----------------------------------------------------------
class IScene
{
public:
	IScene(Game* game);
	virtual ~IScene();
	IScene(const IScene &);

	virtual void				OnSceneStart() = 0;
	virtual IScene				*Clone() = 0;
	virtual void				PreUpdate(float dt);
	virtual void				Update(float dt);

	virtual void				Render(RenderSystem *renderSystem);


protected:
	//SCamera					m_camera;
	Game*					m_game;

};

//----------------------------------------------------------
