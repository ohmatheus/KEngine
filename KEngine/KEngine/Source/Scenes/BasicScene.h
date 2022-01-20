#pragma once

#include "IScene.h"

class IObject;
class BasicObject;
class ModeledObject;

class DirectionalLight;
class PointLight;
class SpotLight;

enum class ESceneMode
{
	Normal,
	Depth,
	ESceneMode_COUNT
};

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
	virtual void				OnKeyEvent(int key, int scancode, int action, int mods);

private:
	std::vector<ModeledObject*>	m_lightCastedObjects;

	DirectionalLight*			m_dirLight;
	std::vector<PointLight*>	m_pointLights;
	std::vector<SpotLight*>		m_spotLights;

	ESceneMode					m_sceneMode;
};