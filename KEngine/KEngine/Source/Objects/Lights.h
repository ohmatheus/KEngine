#pragma once

#include "IObject.h"
#include "ModeledObject.h"

//----------------------------------------------------------
class	ILight : public ModeledObject
{
public:
	ILight();

	glm::vec3			m_ambient;
	glm::vec3			m_diffuse;
	glm::vec3			m_specular;

	virtual void		OnSceneBuild() {}
	virtual void		OnSceneStart() {}
	virtual void		Update(float dt) {}
	virtual void		Render(RenderSystem*, Camera* , GLShader*);
};

//----------------------------------------------------------
class	DirectionalLight : public ILight
{
public:
	glm::vec3			m_direction;
};

//----------------------------------------------------------
class PointLight : public ILight
{
public:
	float 				m_constant;
	float 				m_linear;
	float 				m_quadratic;

	virtual void		OnSceneStart();
	virtual void		Update(float dt);

private:
	glm::vec3			m_initialPosition;
};

//----------------------------------------------------------
class SpotLight : public PointLight
{
public:
	glm::vec3			m_direction;
	float 				m_cutoff;
	float 				m_outerCutoff;
	float 				m_constant;
	float 				m_linear;
	float 				m_quadratic;

	virtual void		OnSceneStart();
	virtual void		Update(float dt);

private:
	glm::vec3			m_initialPosition;

};
