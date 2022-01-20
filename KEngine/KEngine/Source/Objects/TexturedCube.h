#pragma once

#include "IObject.h"

class Camera;

//----------------------------------------------------------
class TexturedCube : public IObject
{
	typedef IObject super;
public:
	TexturedCube();
	virtual ~TexturedCube();

	virtual void			OnSceneBuild();
	virtual void			OnSceneStart();
	virtual void			Update(float dt);
	virtual void			Render(RenderSystem*, Camera*, GLShader*);

private:
};
