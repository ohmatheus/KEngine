#pragma once


//----------------------------------------------------------
class Camera final
{
public:
	enum ECamMode
	{
		Orbit = 0,
		Free3D
	};

	Camera();
	~Camera();

	float			Fov() { return m_fov; }
	glm::vec3&		Position() { return m_position; }
	glm::vec3		Forward();

	void			SetViewProj(const glm::mat4& m) { m_viewProj = m; }
	glm::mat4		ViewProj() { return m_viewProj; }
	void			SetOrthoMat(const glm::mat4& m) { m_orthoMat = m; }
	glm::mat4		OrthoMat() { return m_orthoMat; }
	void			SetProjMat(const glm::mat4& m) { m_projMat = m; }
	glm::mat4		ProjMat() { return m_projMat; }

	glm::mat4		GetViewM();
	void			Update(float dt);

	void			OnKeyEvent(int key, int scancode, int action, int mods);
	void			OnMouseMoved(float xpos, float ypos);
	void			OnMouseKeyEvent(int button, int action, int mods);
	void			OnScrollMoved(float xoffset, float yoffset);

	void			Pan(float pan_factorX, float pan_factorY);
	void			Rotate(glm::vec3 rotation_center, float angle_X_inc, float angle_Y_inc);

	void			RotatePhi(const float radians);
	void			RotateTheta(const float radians);
	void			Zoom(const float by);
	glm::vec3		GetOrbitPosition() const;

	void			SwitchMode();
	void			ResetCamera();

private:
	ECamMode		m_mode;
	float			m_fov;

	glm::f64vec2	m_cursorPosition;

	glm::vec3		m_position;
	glm::vec3		m_direction;
					
	glm::vec3		m_lookAtPos; // center
	glm::vec3		m_up;

	glm::vec3		m_deltaRotation;

	glm::mat4		m_viewProj;
	glm::mat4		m_orthoMat;
	glm::mat4		m_projMat;

	float			m_radius;
	float			m_minRadius;
	float			m_phi; // azimuth
	float			m_theta; // polar

	bool			lookAtMove;
	bool			m_goForw;
	bool			m_goBack;
	bool			m_goRight;
	bool			m_goLeft;
	bool			m_goUp;
	bool			m_goDown;
};