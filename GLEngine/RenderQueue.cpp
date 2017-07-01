#include "stdafx.h"
#include "RenderQueue.h"


namespace GLEngine
{
	RenderQueue::RenderQueue()
	{
		_renderables = list<Renderable*>();
	}


	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::ClearRenderables()
	{
		_renderables.clear();
	}

	void RenderQueue::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		SetGpuState();

		for each (Renderable* currentRenderable in _renderables)
		{
			currentRenderable->Render(sceneManager, graphicsResourceManager);
		}

		ResetGpuDefaultState();
	}
}