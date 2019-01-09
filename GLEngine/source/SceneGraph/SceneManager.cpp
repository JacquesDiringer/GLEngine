#include <stdafx.h>
#include <SceneGraph\SceneManager.h>
#include <Render\RenderingElements.h>
#include <Actor\Actor.h>


namespace GLEngine
{
	SceneManager::SceneManager()
	{
		_rootNode = new SceneNode();
		_updateVisitor = new UpdateVisitor();

		_actorElements = std::unordered_set<Actor*>();
	}


	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Update()
	{
		// Store the time span since last update, to be able to give a delta time to SceneElements to update.
		_updateVisitor->RegisterTime();

		// Only browse through the Actor elements for update.
		for (Actor * currentActor : _actorElements)
		{
			currentActor->Accept(_updateVisitor);
		}
	}
}