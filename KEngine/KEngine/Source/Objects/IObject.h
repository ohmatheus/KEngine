#pragma once

#include "GLShader.h"
#include "TextureResource.h"
#include "RenderSystem.h"

class Camera;

//----------------------------------------------------------
class IObject
{
public:
	IObject();
	virtual ~IObject() {}

	virtual void			OnSceneBuild() = 0;
	virtual void			OnSceneStart() = 0;
	virtual void			Update(float dt) = 0;
	virtual void			Render(RenderSystem*, Camera*, GLShader* = nullptr) = 0;

	const glm::mat4			ModelWorldMatrix();

	glm::vec3&				Position() { return m_position; }
	glm::vec3&				Rotation() { return m_rotation; }
	glm::vec3&				Scale() { return m_scale; }
	glm::vec3&				Color() { return m_color; }

	const std::string&		ShaderName() { return m_shaderName; }
	void					SetShaderName(const char* name) { m_shaderName = name; }

protected:
	glm::vec3				m_position; // x y z
	glm::vec3				m_rotation; // pitch yaw roll
	glm::vec3				m_scale;
	glm::vec3				m_color;
	std::string				m_shaderName; // TODO replace by an ID
};
