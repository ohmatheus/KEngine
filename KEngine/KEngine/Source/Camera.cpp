#include "stdafx.h"

#include "Camera.h"

//----------------------------------------------------------
Camera::Camera()
{
	m_mode = LookAtCenter;
	m_fov = 60.f;
	m_position = glm::vec3(0.f, 0.f, -2.f);
	m_rotation = glm::vec3(0.f);

	m_viewProj = glm::mat4(1.f);
	m_orthoMat = glm::mat4(1.f);
	m_projMat = glm::mat4(1.f);

	m_lookAtPos = glm::vec3(0.0, 0.0, 0.0);

	radius = 5.f;
}

//----------------------------------------------------------
Camera::~Camera()
{}

//----------------------------------------------------------
glm::mat4		Camera::GetViewM()
{
	return m_viewProj;
}

//----------------------------------------------------------
void		Camera::Update(float dt)
{
	static float sdf = 0;
	sdf += dt;

	m_position.x = sin(m_rotation.x) * radius;
	m_position.z = cos(sdf) * radius;
	m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_lookAtPos, glm::vec3(0.0, 1.0, 0.0));
}

//----------------------------------------------------------
void		Camera::OnKeyEvent(int key, int scancode, int action, int mods)
{

}

//----------------------------------------------------------
void		Camera::OnMouseMoved(float xpos, float ypos)
{

}

//----------------------------------------------------------
void		Camera::OnMouseKeyEvent(int button, int action, int mods)
{

}

//----------------------------------------------------------
void		Camera::OnScrollMoved(float xoffset, float yoffset)
{
	if (m_mode == LookAtCenter)
	{
		radius -= yoffset;

	}
}

