#include "stdafx.h"
#include "Renderable.h"


namespace GLEngine
{
	Renderable::Renderable()
	{
	}

	Renderable::~Renderable()
	{
	}

	void Renderable::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}
}