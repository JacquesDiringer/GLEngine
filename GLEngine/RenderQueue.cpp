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

	void RenderQueue::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager) const
	{
		SetGpuState();

		for each (Renderable* currentRenderable in _renderables)
		{
			currentRenderable->Render(sceneManager, graphicsResourceManager);
		}

		ResetGpuDefaultState();
	}
}