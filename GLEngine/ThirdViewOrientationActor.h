#pragma once
#include "Actor.h"

namespace GLEngine
{
	class ThirdViewOrientationActor :
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