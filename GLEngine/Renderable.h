#pragma once

#include "SceneManager.h"

namespace GLEngine
{
	class SceneManager;

	class Renderable
	{
	public:
		Renderable();
		virtual ~Renderable();

		virtual void Render(SceneManager* sceneManager) = 0;
	};
}