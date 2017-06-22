#pragma once

#include "SceneManager.h"
#include "RenderVisitor.h"
#include "RenderableCollectorVisitor.h"

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
	};
}