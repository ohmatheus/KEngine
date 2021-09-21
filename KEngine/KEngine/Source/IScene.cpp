#include "stdafx.h"

#include "IScene.h"
#include "Game.h"

//----------------------------------------------------------
IScene::IScene(Game* game)
: m_game(game)
{
	m_game->AddScene(this);
}

//----------------------------------------------------------
IScene::~IScene()
{
}

//----------------------------------------------------------
IScene::IScene(const IScene &from)
{
}

//----------------------------------------------------------
void		IScene::PreUpdate(float dt)
{
}

//----------------------------------------------------------
void		IScene::Update(float dt)
{
}

//----------------------------------------------------------
void		IScene::Render(RenderSystem *renderSystem)
{
}

//----------------------------------------------------------