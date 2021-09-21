#include "stdafx.h"

#include "IScene.h"
#include "Game.h"

//----------------------------------------------------------
IScene::IScene(Game* game)
: m_game(game)
{}

//----------------------------------------------------------
IScene::~IScene()
{
}

//----------------------------------------------------------
IScene::IScene(const IScene& other)
{
	m_camera = other.m_camera;
	m_game = other.m_game;
}

//----------------------------------------------------------
IScene* IScene::Clone()
{
	return nullptr;
}

//----------------------------------------------------------
void	IScene::OnKeyEvent(int key, int scancode, int action, int mods)
{}

//----------------------------------------------------------
void	IScene::OnMouseMoved(float xpos, float ypos)
{}

//----------------------------------------------------------
void	IScene::OnMouseKeyEvent(int button, int action, int mods)
{}

//----------------------------------------------------------
void	IScene::OnScrollMoved(float xoffset, float yoffset)
{}

//----------------------------------------------------------
