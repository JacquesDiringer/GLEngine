#pragma once
#include "Actor.h"
#include "Quaternion.h"

using Math::Quaternion;

namespace GLEngine
{
	class SpinnerActor :
		public Actor
	{
	public:
		SpinnerActor();
		SpinnerActor(Quaternion rotation);
		~SpinnerActor();

		virtual void Increment(float deltaTime);

	private:
		Quaternion _rotation;
	};
}

