#pragma once
#include <Actor\Actor.h>
#include <Math\Quaternion.h>
#include <Math\Matrix4.h>

using Math::Quaternion;
using Math::Matrix4;

namespace GLEngine
{
	class SpinnerActor :
		public Actor
	{
	public:
		SpinnerActor();
		SpinnerActor(Quaternion* quaternion);
		SpinnerActor(Matrix4* rotationMatrix);
		~SpinnerActor();

		virtual void Increment(float deltaTime);

	private:
		Quaternion* _quaternion = nullptr;
		Matrix4* _rotationMatrix = nullptr;
		bool _quaternionConstructor;
	};
}
