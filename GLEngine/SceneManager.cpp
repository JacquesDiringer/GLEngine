#include "stdafx.h"
#include "SceneManager.h"


namespace GLEngine
{
	SceneManager::SceneManager()
	{
		_rootNode = new SceneNode();
	}


	SceneManager::~SceneManager()
	{
	}
	void SceneManager::Update()
	{
		
	}

	RenderingElements * SceneManager::CollectElementsForRendering()
	{
		return nullptr;
	}
}