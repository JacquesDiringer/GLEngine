#include "stdafx.h"
#include "Actor.h"
#include "SceneElementVisitor.h"

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

	void Actor::Increment(float deltaTime)
	{
	}
}