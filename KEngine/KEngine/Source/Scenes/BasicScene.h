#pragma once

#include "IScene.h"

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
	glm::vec3			m_Position;
	glm::vec3			m_Rotation;
	glm::vec3			m_Scale;
	float				m_Pitch;
	float				m_Yaw;
	float				m_Roll;
};