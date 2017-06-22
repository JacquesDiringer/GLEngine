#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"


namespace GLEngine
{
	RenderManager::RenderManager()
	{
		_renderVisitor = new RenderVisitor();
		_collectorVisitor = new RenderableCollectorVisitor();
	}


	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Render(SceneManager * sceneManager)
	{
		_renderVisitor->SetSceneManager(sceneManager);

		// Triggers renderable elements collection.
		sceneManager->GetRootNode()->Accept(_collectorVisitor);
		RenderingElements* collection = _collectorVisitor->GetCollectedElements();


		// Render elements in the right order.

		// Sky goes first.
		EnvironmentMapSky* sky = collection->GetSky();
		if (sky != nullptr)
		{
			sky->Render(sceneManager);
		}

		// Then models.
		for (Model* currentModel = collection->PopModel(); currentModel != nullptr; currentModel = collection->PopModel())
		{
			currentModel->Render(sceneManager);
		}
	}

}