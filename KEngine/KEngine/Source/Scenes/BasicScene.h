#pragma once

#include "IScene.h"

class IObject;
class BasicObject;

class DirectionalLight;
class PointLight;
class SpotLight;

class BasicScene : public IScene
{
	typedef IScene super;
public:
	BasicScene(Game*);
	virtual ~BasicScene();
	BasicScene(BasicScene const&);

	virtual IScene*				Clone();
	virtual void				BuildScene();
	virtual void				OnSceneStart();
	virtual void				Update(float dt);
	virtual void				Render();

	void						CreateLights();

	virtual void				OnScrollMoved(float xoffset, float yoffset);

private:
	std::vector<IObject*>		m_Objects;
	std::vector<BasicObject*>	m_lightCastedObjects;

	DirectionalLight*			m_dirLight;
	std::vector<PointLight*>	m_pointLights;
	std::vector<SpotLight*>		m_spotLights;
};