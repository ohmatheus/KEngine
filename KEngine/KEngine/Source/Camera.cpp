#include "stdafx.h"

#include "Camera.h"
#include "Game.h"
#include "KWindow.h"

//----------------------------------------------------------
Camera::Camera()
{
	m_mode = Orbit;
	ResetCamera();
	m_fov = 60.f;
	lookAtMove = false;
}

//----------------------------------------------------------
Camera::~Camera()
{}

//----------------------------------------------------------
glm::vec3		Camera::Forward()
{
	return m_lookAtPos - m_position;
}

//----------------------------------------------------------
glm::mat4		Camera::GetViewM()
{
	return m_viewProj;
}

//----------------------------------------------------------
void		Camera::Update(float dt)
{
	std::cout << "phi : " << m_phi << std::endl;
	std::cout << "theta : " << m_theta << std::endl;

	if (m_mode == Orbit)
	{
		RotatePhi(-m_deltaRotation.x * dt);
		RotateTheta(-m_deltaRotation.y * dt);

		m_position = GetOrbitPosition();
		m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_lookAtPos, glm::vec3(0.0, 1.0, 0.0));

		m_direction = glm::normalize(m_position - m_lookAtPos);

	}
	else if (m_mode == Free3D)
	{
		const float sensibilityRot = 0.33f;
		const float sensibilityTrans = 5.f;

		m_phi += -m_deltaRotation.x * dt * sensibilityRot;
		m_theta += m_deltaRotation.y * dt * sensibilityRot;

		if (m_theta > 89.0f * PI / 180.f)
			m_theta = 89.0f * PI / 180.f;
		if (m_theta < -89.0f * PI / 180.f)
			m_theta = -89.0f * PI / 180.f;

		m_direction.x = cos(m_phi) * cos(m_theta);
		m_direction.y = sin(m_theta);
		m_direction.z = sin(m_phi) * cos(m_theta);

		m_direction = glm::normalize(m_direction);

		m_up = glm::normalize(glm::cross(glm::vec3(1.f, 0.f, 0.f), m_direction));

		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_direction));

		if (m_goForw)
			m_position += m_direction * dt * sensibilityTrans;
		if (m_goBack)
			m_position -= m_direction * dt * sensibilityTrans;

		if (m_goRight)
			m_position -= right * dt * sensibilityTrans;
		if (m_goLeft)
			m_position += right * dt * sensibilityTrans;

		m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_position + m_direction, glm::vec3(0.0, 1.0, 0.0));
	}

	m_deltaRotation = glm::vec3(0.f);
}

//----------------------------------------------------------
void		Camera::OnKeyEvent(int key, int scancode, int action, int mods)
{
	if (m_mode == Free3D)
	{
		GLFWwindow* window = Game::Instance()->GetRenderWindow()->Window();

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			m_goForw = true;
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			m_goForw = false;

		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			m_goBack = true;
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			m_goBack = false;

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			m_goRight = true;
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
			m_goRight = false;

		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			m_goLeft = true;
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
			m_goLeft = false;
	
	}
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
	if (m_mode == Orbit)
		Zoom(yoffset);
}

//----------------------------------------------------------
void		Camera::RotatePhi(const float radians)
{
	m_phi += radians;

	const auto fullCircle = 2.0f * PI;
	m_phi = fmodf(m_phi, fullCircle);
	if (m_phi < 0.0f)
		m_phi = fullCircle + m_phi;
}

//----------------------------------------------------------
void		Camera::RotateTheta(const float radians)
{
	m_theta += radians;

	const auto polarCap = PI / 2.0f - 0.001f;
	if (m_theta > polarCap)
		m_theta = polarCap;

	if (m_theta < -polarCap)
		m_theta = -polarCap;
}

//----------------------------------------------------------
void		Camera::Zoom(const float by)
{
	m_radius -= by;
	if (m_radius < m_minRadius)
		m_radius = m_minRadius;
}

//----------------------------------------------------------
glm::vec3	Camera::GetOrbitPosition() const
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

//----------------------------------------------------------
void		Camera::SwitchMode()
{
	if (m_mode == Orbit)
	{
		m_mode = Free3D;
		m_phi -= PI;
		m_theta = -m_theta;
	}
	else if (m_mode == Free3D)
	{
		m_mode = Orbit;
		ResetCamera();
	}
}

//----------------------------------------------------------
void		Camera::ResetCamera()
{
	m_position = glm::vec3(0.f, 0.f, -2.f);

	m_viewProj = glm::mat4(1.f);
	m_orthoMat = glm::mat4(1.f);
	m_projMat = glm::mat4(1.f);

	m_lookAtPos = glm::vec3(0.0, 0.0, 0.0);

	m_phi = 90.f * PI / 180.f;
	m_theta = 0.f;

	m_radius = 5.f;
	m_minRadius = 1.f;
	m_viewProj = glm::lookAt(glm::vec3(m_position.x, m_position.y, m_position.z), m_lookAtPos, glm::vec3(0.0, 1.0, 0.0));
}

