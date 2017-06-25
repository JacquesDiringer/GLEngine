#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"


namespace GLEngine
{
	RenderManager::RenderManager()
	{
		_renderVisitor = new RenderVisitor();
		_collectorVisitor = new RenderableCollectorVisitor();
		_modelsRenderQueue = new NoAlphaRenderQueue();
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

		// Then the models.
		// Fill the models render queue.
		for (Model* currentModel = collection->PopModel(); currentModel != nullptr; currentModel = collection->PopModel())
		{
			_modelsRenderQueue->AddRenderable(currentModel);
		}

		// Then render it.
		_modelsRenderQueue->Render(sceneManager);

		// Then clear it.
		_modelsRenderQueue->ClearRenderables();
	}

}