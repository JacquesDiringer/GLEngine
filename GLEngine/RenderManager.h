#pragma once

#include "SceneManager.h"
#include "RenderVisitor.h"

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
	};
}