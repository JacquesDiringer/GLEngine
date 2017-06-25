#include "stdafx.h"
#include "SceneManager.h"
#include "RenderingElements.h"


namespace GLEngine
{
	SceneManager::SceneManager()
	{
		_rootNode = new SceneNode();
		_updateVisitor = new UpdateVisitor();
	}


	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Update()
	{
		// Store the time span since last update, to be able to give a delta time to SceneElements to update.
		_updateVisitor->RegisterTime();

		// Update, starting from the root node.
		_rootNode->Accept(_updateVisitor);
	}
}