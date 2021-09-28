#include "stdafx.h"

#include "BasicObject.h"
#include "IScene.h"

//----------------------------------------------------------
BasicObject::BasicObject()
:	super()
{

}

//----------------------------------------------------------
BasicObject::~BasicObject()
{

}

//----------------------------------------------------------
void		BasicObject::OnSceneBuild()
{

}

//----------------------------------------------------------
void		BasicObject::OnSceneStart()
{

}

//----------------------------------------------------------
void		BasicObject::Update(float dt)
{

}

//----------------------------------------------------------
void		BasicObject::Render(RenderSystem* rS, SCamera* cam)
{
	glm::mat4		modelW = ModelWorldMatrix();

	GLShader* shader = rS->GetShader(m_ShaderName);
	MeshData* mesh = rS->GetMesh(m_MeshName);

	TextureResource* containerResource = rS->GetTexture("container");
	TextureResource* smileyTex = rS->GetTexture("smiley");

	shader->Bind();

	glUniformMatrix4fv(shader->Uniform("model"), 1, GL_FALSE, glm::value_ptr(modelW));
	glUniformMatrix4fv(shader->Uniform("view"), 1, GL_FALSE, glm::value_ptr(cam->GetView()));
	glUniformMatrix4fv(shader->Uniform("proj"), 1, GL_FALSE, glm::value_ptr(cam->m_ProjMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerResource->TextureRenderId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, smileyTex->TextureRenderId());
	glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture1"), 0); // set it manually
	glUniform1i(glGetUniformLocation(shader->ProgramID(), "texture2"), 1); // set it manually

	glBindVertexArray(mesh->VAO());

	//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO()); // is it necessary to couple that with glBindVertexArray ?
	glDrawArrays(mesh->Mode(), 0, mesh->VerticesNbr());
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	shader->Unbind();
}
