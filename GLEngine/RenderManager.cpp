#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

namespace GLEngine
{
	RenderManager::RenderManager()
	{
		_collectorVisitor = new RenderableCollectorVisitor();
		_modelsRenderQueue = new NoAlphaRenderQueue();
		_skyRenderQueue = new SkyRenderQueue();
	}


	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		double timeAtRenderStart = glfwGetTime();

		// Triggers renderable elements collection.
		sceneManager->GetRootNode()->Accept(_collectorVisitor);
		RenderingElements* collection = _collectorVisitor->GetCollectedElements();


		// Render elements in the right order.

		// Sky goes first.
		EnvironmentMapSky* sky = collection->GetSky();
		if (sky != nullptr)
		{
			_skyRenderQueue->AddRenderable(sky);

			_skyRenderQueue->Render(sceneManager, graphicsResourceManager);
		}

		// Then the models.
		// Fill the models render queue.
		for (InstancedModel* currentInstancedModel = collection->PopInstancedModel(); currentInstancedModel != nullptr; currentInstancedModel = collection->PopInstancedModel())
		{
			_modelsRenderQueue->AddRenderable(currentInstancedModel);
		}

		// Then render it.
		_modelsRenderQueue->Render(sceneManager, graphicsResourceManager);

		// Clear render queues.
		_skyRenderQueue->ClearRenderables();
		_modelsRenderQueue->ClearRenderables();

		// TODO: this call to clear will not stay necessary, each list should have had all it's elements popped at the end of the render loop.
		collection->Clear();

		double timeAtRenderEnd = glfwGetTime();

		double renderTime = timeAtRenderEnd - timeAtRenderStart;

		if (_frameCount%10 == 0)
		{
			std::cout << "Render time: " << renderTime * 1000 << "ms; FPS: " << 1/renderTime << std::endl;
		}

		++_frameCount;
	}

}