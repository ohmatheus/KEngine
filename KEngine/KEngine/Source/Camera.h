#pragma once


enum ECamMode
{
	LookAtCenter,
	Free3D
};

class Camera final
{
public:
	Camera();
	~Camera();

private:
	ECamMode	m_mode = LookAtCenter;
};