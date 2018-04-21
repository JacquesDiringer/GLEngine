#pragma once
#include <Actor\Actor.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API ThirdViewOrientationActor :
		public Actor
	{
	public:
		ThirdViewOrientationActor();
		ThirdViewOrientationActor(SceneNode* target);
		~ThirdViewOrientationActor();

		virtual void Increment(float deltaTime);

	private:
		SceneNode* _target = nullptr;
	};

}