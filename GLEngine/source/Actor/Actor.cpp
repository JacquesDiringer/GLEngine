#include <stdafx.h>
#include <Actor\Actor.h>
#include <SceneGraph\SceneElementVisitor.h>

namespace GLEngine
{
	Actor::Actor()
		: SceneElement()
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}
}