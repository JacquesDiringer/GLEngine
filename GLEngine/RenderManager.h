#pragma once

#include "SceneManager.h"
#include "RenderVisitor.h"
#include "RenderableCollectorVisitor.h"
#include "NoAlphaRenderQueue.h"
#include "SkyRenderQueue.h"

namespace GLEngine
{
	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void Render(SceneManager* sceneManager);

	private:
		RenderVisitor* _renderVisitor;
		RenderableCollectorVisitor* _collectorVisitor;
		NoAlphaRenderQueue* _modelsRenderQueue;
		SkyRenderQueue* _skyRenderQueue;
		int _frameCount = 0;
	};
}