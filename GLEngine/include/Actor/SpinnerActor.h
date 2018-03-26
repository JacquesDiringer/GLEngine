#pragma once
#include <Actor\Actor.h>
#include <Math\Quaternion.h>
#include <Math\Matrix4.h>

using GLEngineMath::Quaternion;
using GLEngineMath::Matrix4;

namespace GLEngine
{
	class SpinnerActor :
		public Actor
	{
	public:
		SpinnerActor();
		SpinnerActor(const Quaternion& quaternion);
		SpinnerActor(const Matrix4& rotationMatrix);
		~SpinnerActor();

		virtual void Increment(float deltaTime);

	private:
		Quaternion _quaternion;
		Matrix4 _rotationMatrix;
		bool _quaternionConstructor;
	};
}

