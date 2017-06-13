#pragma once

#include "SceneManager.h"

namespace GLEngine
{
	class Renderable
	{
	public:
		Renderable();
		~Renderable();

		virtual void Render(SceneManager* sceneManager) = 0;
	};
}