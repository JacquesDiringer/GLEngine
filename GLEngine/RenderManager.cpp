#include "stdafx.h"
#include "RenderManager.h"


namespace GLEngine
{
	RenderManager::RenderManager()
	{
		_renderVisitor = new RenderVisitor();
	}


	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Render(SceneManager * sceneManager)
	{
		_renderVisitor->SetSceneManager(sceneManager);
		sceneManager->GetRootNode()->Accept(_renderVisitor);
	}

}