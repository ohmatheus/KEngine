#include "stdafx.h"

#include "TexturedCube.h"
#include "IScene.h"

//----------------------------------------------------------
TexturedCube::TexturedCube()
	: super()
{

}

//----------------------------------------------------------
TexturedCube::~TexturedCube()
{

}

//----------------------------------------------------------
void		TexturedCube::OnSceneBuild()
{

}

//----------------------------------------------------------
void		TexturedCube::OnSceneStart()
{

}

//----------------------------------------------------------
void		TexturedCube::Update(float dt)
{

}

//----------------------------------------------------------
void		TexturedCube::Render(RenderSystem* rS, Camera* cam)
{
	glm::mat4		modelW = ModelWorldMatrix();

	GLShader* shader = rS->GetShader(m_shaderName);
	MeshData* mesh = rS->GetMesh(m_meshName);

	TextureResource* containerResource = rS->GetTexture("container");
	TextureResource* smileyTex = rS->GetTexture("smiley");

	shader->Bind();

	glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
	glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(cam->GetViewM()));
	glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(cam->ProjMat()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerResource->TextureRenderId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, smileyTex->TextureRenderId());
	glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture1"), 0); // set it manually
	glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture2"), 1); // set it manually

	glBindVertexArray(mesh->VAO());
	glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	shader->Unbind();
}
