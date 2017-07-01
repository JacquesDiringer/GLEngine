#pragma once

#include "SceneManager.h"
#include "GraphicsResourceManager.h"

namespace GLEngine
{
	class SceneManager;

	class Renderable
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) = 0;
	};
}