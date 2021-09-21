#include "stdafx.h"

#include "BasicScene.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"

#include "GLShader.h"
#include "MeshData.h"

//----------------------------------------------------------
BasicScene::BasicScene(Game* game)
:	IScene(game)
{

}

//----------------------------------------------------------
BasicScene::~BasicScene()
{

}

//----------------------------------------------------------
BasicScene::BasicScene(BasicScene const& other)
:	super(other)
{

}

//----------------------------------------------------------
IScene* BasicScene::Clone()
{
	BasicScene* clone = new BasicScene(*this);
	return clone;
}

//----------------------------------------------------------
void	BasicScene::BuildScene()
{
	m_ShaderName = "DefaultShader";
	m_MeshName = "Rectangle";
	m_Color = glm::vec4(1.f, 0.f, 1.f, 1.f);

	m_Position = glm::vec3(0.f);
	m_Rotation = glm::vec3(0.f);
	m_Scale = glm::vec3(1.f);

	m_Yaw = 0.f;
	m_Pitch = 0.f;
	m_Roll = 0.f;
}

//----------------------------------------------------------
void	BasicScene::OnSceneStart()
{

}

//----------------------------------------------------------
void	BasicScene::Update(float dt)
{

}

//----------------------------------------------------------
void	BasicScene::Render()
{
	{
		m_camera.m_OrthoMat = glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f);
		m_camera.m_ProjMat = glm::perspective(glm::radians(m_camera.m_Fov), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f);

		m_camera.m_ViewProj = m_camera.m_ProjMat * m_camera.GetView(); // what purpose ?
	}

	{
		GLShader* shader = m_game->GetRenderSystem()->GetShader(m_ShaderName);
		MeshData* mesh = m_game->GetRenderSystem()->GetMesh(m_MeshName);
		IScene* currentScene = m_game->GetRenderSystem()->GetGame()->CurrentScene();

		glm::mat4		modelW;
		{
			glm::mat4 result = glm::mat4(1.f);

			result = glm::translate(result, m_Position);
			result = glm::rotate(result, glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			result = glm::rotate(result, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			result = glm::rotate(result, glm::radians(m_Roll), glm::vec3(0.0f, 0.0f, 1.0f));
			result = glm::scale(result, m_Scale);

			modelW = result;

			//if (m_Parent != nullptr && worldspace)
			//	return m_Parent->ModelMatrix(worldspace) * result;
			//else
			//	return result;
		}
		assert(shader != nullptr);
		assert(mesh != nullptr);

		shader->Bind();
		glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
		glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(m_camera.GetView()));
		glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(m_camera.m_ProjMat));
		glUniform4fv(shader->Uniform("uColor"), 1, glm::value_ptr(m_Color));
		glBindVertexArray(mesh->VAO());
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO());
		glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
		glBindVertexArray(0);
		glEnableVertexAttribArray(0);
		shader->Unbind();
	}
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	m_camera.m_Position.z += yoffset;
}
