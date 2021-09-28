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

	{
		BasicObject* obj = new BasicObject();
		obj->SetShaderName("TexturedMeshShader");
		obj->SetMeshName("TexturedCube");
		obj->Position().x = 2.f;
		obj->Rotation().y = 40.f;
		m_Objects.push_back(obj);
	}
	
}

//----------------------------------------------------------
void	BasicScene::OnSceneStart()
{}

//----------------------------------------------------------
void	BasicScene::Update(float dt)
{}

//----------------------------------------------------------
void	BasicScene::Render()
{
	{
		m_camera.m_OrthoMat = glm::ortho(-400.f, 400.0f, -300.f, 300.0f, 0.1f, 200.0f);
		m_camera.m_ProjMat = glm::perspective(glm::radians(m_camera.m_Fov), (float)m_game->GetRenderWindow()->Width() / (float)m_game->GetRenderWindow()->Height(), 0.1f, 200.0f);
		m_camera.m_ViewProj = m_camera.m_ProjMat * m_camera.GetView(); // what purpose ?
	}

	for (int i = 0; i < m_Objects.size(); i++)
		m_Objects[i]->Render(m_game->GetRenderSystem(), &m_camera);
}

//----------------------------------------------------------
void	BasicScene::OnScrollMoved(float xoffset, float yoffset)
{
	m_camera.m_Position.z += yoffset;
}
