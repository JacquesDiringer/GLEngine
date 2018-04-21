#pragma once

#include <SceneGraph\SceneElement.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API Actor :
		public SceneElement
	{
	public:
		Actor();
		~Actor();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Increment(float deltaTime) = 0;
	};

}
