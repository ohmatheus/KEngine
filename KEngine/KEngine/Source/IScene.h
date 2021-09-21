#pragma once

#include <vector>
#include <string>

//----------------------------------------------------------
class	RenderSystem;
class	Game;

//----------------------------------------------------------
struct	SCamera
{
	float		m_Fov = 60.f;
	glm::vec3	m_Position = glm::vec3(0.f, 0.f, -2.f);
	glm::vec3	m_Rotation = glm::vec3(0.f);

	glm::mat4	m_ViewProj = glm::mat4(1.f);
	glm::mat4	m_OrthoMat = glm::mat4(1.f);
	glm::mat4	m_ProjMat = glm::mat4(1.f);

	glm::mat4	GetView()
	{
		glm::mat4 result = glm::mat4(1.f);

		result = glm::translate(result, m_Position);
		//result = glm::rotate(result, m_Rotation);
		return result;
	}
};

//----------------------------------------------------------
class IScene
{
public:
	IScene(Game* game);
	virtual ~IScene();
	IScene(const IScene&);

	virtual IScene*			Clone() = 0; // could probably do this with crtp to automise with template but... no time
	virtual void			BuildScene() = 0;
	virtual void			OnSceneStart() = 0;
	virtual void			Update(float dt) = 0;
	virtual void			Render() = 0;

	virtual void			OnKeyEvent(int key, int scancode, int action, int mods);
	virtual void			OnMouseMoved(float xpos, float ypos);
	virtual void			OnMouseKeyEvent(int button, int action, int mods);
	virtual void			OnScrollMoved(float xoffset, float yoffset);

protected:
	SCamera					m_camera;
	Game*					m_game;

};

//----------------------------------------------------------
