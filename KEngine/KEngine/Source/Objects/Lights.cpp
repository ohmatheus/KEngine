#include "stdafx.h"

#include "Lights.h"
#include "RenderSystem.h"
#include "Camera.h"

//----------------------------------------------------------
ILight::ILight()
{
	SetMeshName("SimpleCube");
	SetShaderName("LightObject");
	Scale() = glm::vec3(0.2f);
}

//----------------------------------------------------------
void	ILight::Render(RenderSystem* rS, Camera* camera)
{
	glm::mat4		modelW = ModelWorldMatrix();

	GLShader* shader = rS->GetShader(ShaderName());
	MeshData* mesh = rS->GetMesh(MeshName());

	// draw color
	float max = std::max(std::max(m_ambient.x, m_ambient.y), m_ambient.z);
	float inv = 1.f / max;
	glm::vec3 drawColor(m_ambient.x * inv, m_ambient.y * inv, m_ambient.z * inv);

	shader->Bind();

	glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
	glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewM()));
	glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(camera->ProjMat()));
	glUniform3f(shader->Uniform("lightColor"), drawColor.x, drawColor.y, drawColor.z);

	glBindVertexArray(mesh->VAO());

	//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
	glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	shader->Unbind();
}

//----------------------------------------------------------
void	PointLight::OnSceneStart()
{
	ILight::OnSceneStart();
	m_initialPosition = m_Position;
}

//----------------------------------------------------------
void	PointLight::Update(float dt)
{
	static	float angle = 0.f;
	const	float radius = 2.5f;

	const float speed = 0.3f;

	angle += dt * speed;

	Position().x = cosf(angle) * radius;
	Position().y = cosf(angle);
	Position().z = sinf(angle) * radius;

	Position() += m_initialPosition;
}

//----------------------------------------------------------
void	SpotLight::OnSceneStart()
{
	ILight::OnSceneStart();
	m_initialPosition = m_Position;
}

//----------------------------------------------------------
void	SpotLight::Update(float dt)
{
	static	float angle = 0.f;
	const	float radius = 2.5f;

	const float speed = 0.3f;

	angle += dt * speed;

	Position().x = cosf(angle) * radius;
	Position().y = 0.f;
	Position().z = sinf(angle) * radius;

	Position() += m_initialPosition;
}
