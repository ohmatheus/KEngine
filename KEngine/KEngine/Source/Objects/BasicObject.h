#pragma once

#include "IObject.h"

class Camera;

//----------------------------------------------------------
class BasicObject : public IObject
{
	typedef IObject super;
public:
	BasicObject();
	virtual ~BasicObject();

	virtual void			OnSceneBuild();
	virtual void			OnSceneStart();
	virtual void			Update(float dt);
	virtual void			Render(RenderSystem*, Camera*);

private:
};
