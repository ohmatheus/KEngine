#pragma once

#include "IObject.h"

//----------------------------------------------------------
class ModeledObject : public IObject
{
	typedef IObject super;
public:
	ModeledObject();
	virtual ~ModeledObject();

	virtual void			OnSceneBuild() {}
	virtual void			OnSceneStart() {}
	virtual void			Update(float dt) {}
	virtual void			Render(RenderSystem*, Camera*, GLShader*) {}

	void					SetModelName(const std::string& name) { m_modelName = name; }
	const std::string&		ModelName() { return m_modelName; }

private:
	std::string				m_modelName;

};

