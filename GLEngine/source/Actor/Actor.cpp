#include <stdafx.h>
#include <Actor\Actor.h>
#include <SceneGraph\SceneElementVisitor.h>

namespace GLEngine
{
	Actor::Actor(SceneManager& sceneManager)
		: SceneElement(), _sceneManager(sceneManager)
	{
		_sceneManager.AddActor(this);
	}

	Actor::~Actor()
	{
		_sceneManager.RemoveActor(this);
	}

	void Actor::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}
}