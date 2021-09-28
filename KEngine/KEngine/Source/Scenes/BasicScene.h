#pragma once

#include "IScene.h"

class IObject;

class BasicScene : public IScene
{
	typedef IScene super;
public:
	BasicScene(Game*);
	virtual ~BasicScene();
	BasicScene(BasicScene const&);

	virtual IScene*		Clone();
	virtual void		BuildScene();
	virtual void		OnSceneStart();
	virtual void		Update(float dt);
	virtual void		Render();

	virtual void		OnScrollMoved(float xoffset, float yoffset);

private:
	std::vector<IObject*> m_Objects;
};