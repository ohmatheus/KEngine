#include "stdafx.h"

#include "BasicScene.h"

#include "Game.h"
#include "RenderSystem.h"
#include "KWindow.h"
#include "Objects/BasicObject.h"

#include "GLShader.h"
#include "MeshData.h"

#include "stb_image.h"

//----------------------------------------------------------
BasicScene::BasicScene(Game* game)
:	IScene(game)
{}

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
		BasicObject* obj = new BasicObject();
		obj->SetShaderName("TexturedMeshShader");
		obj->SetMeshName("TexturedCube");
		m_Objects.push_back(obj);
	}

	/*{
		BasicObject* obj = new BasicObject();
		obj->SetShaderName("TexturedMeshShader");
		obj->SetMeshName("TexturedCube");
		obj->Position().x = 2.f;
		obj->Rotation().y = 40.f;
		m_Objects.push_back(obj);
	}*/

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

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Update(dt);
}

//----------------------------------------------------------
void	BasicScene::Render()
{
	{
		m_camera.SetOrthoMat(glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f));
		m_camera.SetProjMat(glm::perspective(glm::radians(m_camera.Fov()), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f));
	}

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Render(m_game->GetRenderSystem(), &m_camera);
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	super::OnScrollMoved(xoffset, yoffset);
}
