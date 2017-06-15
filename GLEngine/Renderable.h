#pragma once

#include "SceneManager.h"

namespace GLEngine
{
	class Renderable :
		public SceneElement
	{
	public:
		Renderable();
		~Renderable();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager) = 0;
	};
}