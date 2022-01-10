#include "stdafx.h"

#include "Camera.h"

//----------------------------------------------------------
Camera::Camera()
{
	m_mode = LookAtCenter;
	m_fov = 60.f;
	m_position = glm::vec3(0.f, 0.f, -2.f);

	m_viewProj = glm::mat4(1.f);
	m_orthoMat = glm::mat4(1.f);
	m_projMat = glm::mat4(1.f);

	m_lookAtPos = glm::vec3(0.0, 0.0, 0.0);

	m_phi = 90.f * PI / 180.f;
	m_theta = 0.f;

	m_radius = 5.f;
	m_minRadius = 1.f;
	lookAtMove = false;
	m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_lookAtPos, glm::vec3(0.0, 1.0, 0.0));

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
	RotatePhi(- m_deltaRotation.x * dt);
	RotateTheta(- m_deltaRotation.y * dt);

	m_position = GetOrbitPosition();
	m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_lookAtPos, glm::vec3(0.0, 1.0, 0.0));

	m_deltaRotation = glm::vec3(0.f);
}

//----------------------------------------------------------
void		Camera::OnKeyEvent(int key, int scancode, int action, int mods)
{

}

//----------------------------------------------------------
void		Camera::OnMouseMoved(float deltaX, float deltaY)
{
	if (lookAtMove)
	{
		m_deltaRotation.x += deltaX;
		m_deltaRotation.y += deltaY;
	}
}

//----------------------------------------------------------
void		Camera::OnMouseKeyEvent(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		lookAtMove = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		lookAtMove = false;

}

//----------------------------------------------------------
void		Camera::OnScrollMoved(float xoffset, float yoffset)
{
	if (m_mode == LookAtCenter)
		Zoom(yoffset);
}

//----------------------------------------------------------
void			Camera::RotatePhi(const float radians)
{
	m_phi += radians;

	const auto fullCircle = 2.0f * PI;
	m_phi = fmodf(m_phi, fullCircle);
	if (m_phi < 0.0f)
		m_phi = fullCircle + m_phi;
}

//----------------------------------------------------------
void			Camera::RotateTheta(const float radians)
{
	m_theta += radians;

	const auto polarCap = PI / 2.0f - 0.001f;
	if (m_theta > polarCap)
		m_theta = polarCap;

	if (m_theta < -polarCap)
		m_theta = -polarCap;
}

//----------------------------------------------------------
void			Camera::Zoom(const float by)
{
	m_radius -= by;
	if (m_radius < m_minRadius)
		m_radius = m_minRadius;
}

//----------------------------------------------------------
glm::vec3		Camera::GetOrbitPosition() const
{
	const auto sineAzimuth = sin(m_phi);
	const auto cosineAzimuth = cos(m_phi);
	const auto sinePolar = sin(m_theta);
	const auto cosinePolar = cos(m_theta);

	const auto x = m_lookAtPos.x + m_radius * cosinePolar * cosineAzimuth;
	const auto y = m_lookAtPos.y + m_radius * sinePolar;
	const auto z = m_lookAtPos.z + m_radius * cosinePolar * sineAzimuth;

	return glm::vec3(x, y, z);
}

