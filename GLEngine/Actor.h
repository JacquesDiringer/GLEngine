#pragma once

#include "SceneElement.h"

namespace GLEngine
{
	class Actor :
		public SceneElement
	{
	public:
		Actor();
		~Actor();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Increment(float deltaTime) = 0;
	};

}
