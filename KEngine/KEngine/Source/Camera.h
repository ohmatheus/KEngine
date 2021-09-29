#pragma once


//----------------------------------------------------------
class Camera final
{
public:
	enum ECamMode
	{
		LookAtCenter = 0,
		Free3D
	};

	Camera();
	~Camera();

	float			Fov() { return m_fov; }
	glm::vec3&		Position() { return m_position; }
	glm::vec3&		Rotation() { return m_rotation; }

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

private:
	ECamMode		m_mode;
	float			m_fov;
	glm::vec3		m_position;
	glm::vec3		m_rotation;
	glm::vec3		m_lookAtPos;

	glm::mat4		m_viewProj;
	glm::mat4		m_orthoMat;
	glm::mat4		m_projMat;


	float			radius;

	float			YRot = 0;

};