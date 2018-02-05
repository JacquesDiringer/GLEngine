#pragma once

#include <SceneGraph\SceneManager.h>
#include "GraphicsResourceManager.h"

namespace GLEngine
{
	class Renderable
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const = 0;
	};
}