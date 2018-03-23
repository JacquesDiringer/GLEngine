#pragma once
#include <Instanciater.h>

class GLEngineObjectInstanciater : Generator::Instanciater
{
public:
	GLEngineObjectInstanciater();
	~GLEngineObjectInstanciater();

	virtual void UpdateDisplayables(vector<shared_ptr<Generator::Displayable>> toAdd, vector<shared_ptr<Generator::Displayable>> toRemove);
};
