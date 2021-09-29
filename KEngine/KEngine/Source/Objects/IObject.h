#pragma once

#include "GLShader.h"
#include "MeshData.h"
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
	virtual void			Render(RenderSystem*, Camera*) = 0;

	const glm::mat4			ModelWorldMatrix();

	glm::vec3&				Position() { return m_Position; }
	glm::vec3&				Rotation() { return m_Rotation; }
	glm::vec3&				Scale() { return m_Scale; }
	glm::vec3&				Color() { return m_Color; }

	const std::string&		ShaderName() { return m_ShaderName; }
	void					SetShaderName(const char* name) { m_ShaderName = name; }
	const std::string&		MeshName() { return m_MeshName; }
	void					SetMeshName(const char* name) { m_MeshName = name; }

protected:
	glm::vec3				m_Position; // x y z
	glm::vec3				m_Rotation; // pitch yaw roll
	glm::vec3				m_Scale;
	glm::vec3				m_Color;
	std::string				m_ShaderName; // TODO replace by an ID
	std::string				m_MeshName; // TODO replace by an ID
};
