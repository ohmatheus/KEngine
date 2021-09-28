#include "stdafx.h"

#include "IObject.h"

//----------------------------------------------------------
IObject::IObject()
{
	m_Position = glm::vec3(0.f);
	m_Rotation = glm::vec3(0.f, 0.f, 0.f);
	m_Scale = glm::vec3(1.f);
	m_Color = glm::vec4(1.f, 0.f, 1.f, 1.f);
}

//----------------------------------------------------------
const glm::mat4		IObject::ModelWorldMatrix()
{
	glm::mat4		modelW;
	{
		glm::mat4 result = glm::mat4(1.f);

		result = glm::translate(result, m_Position);
		result = glm::rotate(result, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // pitch
		result = glm::rotate(result, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // yaw
		result = glm::rotate(result, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // roll
		result = glm::scale(result, m_Scale);

		modelW = result;

		//if (m_Parent != nullptr && worldspace)
		//	return m_Parent->ModelMatrix(worldspace) * result;
		//else
		//	return result;
	}

	return modelW;
}
