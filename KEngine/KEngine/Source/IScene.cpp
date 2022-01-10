#include "stdafx.h"

#include "IScene.h"
#include "Game.h"

//----------------------------------------------------------
IScene::IScene(Game* game)
: m_game(game)
{}

//----------------------------------------------------------
IScene::~IScene()
{}

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
{
	m_camera.OnKeyEvent(key, scancode, action, mods);
}

//----------------------------------------------------------
void	IScene::Update(float dt)
{
	m_camera.Update(dt);
}

//----------------------------------------------------------
void	IScene::OnMouseMoved(float deltaX, float deltaY)
{
	m_camera.OnMouseMoved(deltaX, deltaY);
}

//----------------------------------------------------------
void	IScene::OnMouseKeyEvent(int button, int action, int mods)
{
	m_camera.OnMouseKeyEvent(button, action, mods);
}

//----------------------------------------------------------
void	IScene::OnScrollMoved(float xoffset, float yoffset)
{
	m_camera.OnScrollMoved(xoffset, yoffset);
}

