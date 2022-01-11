#include "stdafx.h"

#include "BasicScene.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"
#include "Objects/BasicObject.h"
#include "Objects/TexturedCube.h"

#include "GLShader.h"
#include "MeshData.h"

#include "stb_image.h"

//----------------------------------------------------------
BasicScene::BasicScene(Game* game)
:	IScene(game)
{
	m_lightObject = nullptr;
	m_lightCastedObject = nullptr;
}

//----------------------------------------------------------
BasicScene::~BasicScene()
{
	for (int i = 0; i < m_Objects.size(); i++)
		delete m_Objects[i];
}

//----------------------------------------------------------
BasicScene::BasicScene(BasicScene const& other)
:	super(other)
{}

//----------------------------------------------------------
IScene* BasicScene::Clone()
{
	BasicScene* clone = new BasicScene(*this);
	return clone;
}

//----------------------------------------------------------
void	BasicScene::BuildScene()
{
	{
		IObject* obj = new TexturedCube();
		obj->SetMeshName("TexturedCube");
		obj->SetShaderName("TexturedMeshShader");
		obj->Position() = glm::vec3(-3.f, 0.f, 0.f);
		// set manually texture
		m_Objects.push_back(obj);
	}

	{
		m_lightObject = new BasicObject();
		m_lightObject->SetMeshName("SimpleCube");
		m_lightObject->SetShaderName("LightObject");
		m_lightObject->Scale() = glm::vec3(0.2f);
		m_lightObject->Position() = glm::vec3(1.2f, 1.0f, -2.0f);
	}

	{
		m_lightCastedObject = new BasicObject();
		m_lightCastedObject->SetMeshName("SimpleCube_Normal");
		m_lightCastedObject->SetShaderName("SimpleLightCastedObject");
		m_lightCastedObject->Position() = glm::vec3(0.f, 0.f, -1.0f);
	}

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->OnSceneBuild();
}

//----------------------------------------------------------
void	BasicScene::OnSceneStart()
{
	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->OnSceneStart();
}

//----------------------------------------------------------
void	BasicScene::Update(float dt)
{
	super::Update(dt);

	static float angle = 0.f;
	angle += dt;

	m_lightObject->Position().x = cosf(angle);
	m_lightObject->Position().z = sinf(angle);


	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Update(dt);
}

//----------------------------------------------------------
void	BasicScene::Render()
{
	RenderSystem* rS = m_game->GetRenderSystem();
	glm::vec3	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	{
		m_camera.SetOrthoMat(glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f));
		m_camera.SetProjMat(glm::perspective(glm::radians(m_camera.Fov()), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f));
	}

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Render(rS, &m_camera);

	// render lightObject
	{
		glm::mat4		modelW = m_lightObject->ModelWorldMatrix();

		GLShader* shader = rS->GetShader(m_lightObject->ShaderName());
		MeshData* mesh = rS->GetMesh(m_lightObject->MeshName());

		shader->Bind();

		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewM()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.ProjMat()));
		glUniform3f(shader->Uniform("lightColor"), lightColor.x, lightColor.y, lightColor.z);

		glBindVertexArray(mesh->VAO());

		//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}

	// render lightCastedObject
	{
		glm::mat4			modelW = m_lightCastedObject->ModelWorldMatrix();
		GLShader*			shader = rS->GetShader(m_lightCastedObject->ShaderName());
		MeshData*			mesh = rS->GetMesh(m_lightCastedObject->MeshName());
		TextureResource*	container2tex = rS->GetTexture("container2"); // diffuseMap
		TextureResource*	container2tex_specular = rS->GetTexture("container2_specularMap"); // specularMap

		shader->Bind();

		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewM()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.ProjMat()));

		glUniform3f(shader->Uniform("viewPos"), m_camera.Position().x, m_camera.Position().y, m_camera.Position().z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container2tex->TextureRenderId());
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "material.diffuse"), 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container2tex_specular->TextureRenderId());
		glUniform1i(glGetUniformLocation(shader->ProgramID(), "material.specular"), 1);

		glUniform1f(shader->Uniform("material.shininess"), 32.f);

		glUniform3f(shader->Uniform("light.position"), m_lightObject->Position().x, m_lightObject->Position().y, m_lightObject->Position().z);
		glUniform3f(shader->Uniform("light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(shader->Uniform("light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(shader->Uniform("light.specular"), 1.0f, 1.0f, 1.0f);



		glBindVertexArray(mesh->VAO());

		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	super::OnScrollMoved(xoffset, yoffset);
}
