#pragma once

//#include "SceneManager.h"
#include "SceneElement.h"

namespace GLEngine
{
	class SceneManager;

	class Renderable :
		public SceneElement
	{
	public:
		Renderable();
		~Renderable();

		virtual void Accept(SceneElementVisitor* visitor) = 0;

		virtual void Render(SceneManager* sceneManager) = 0;
	};
}