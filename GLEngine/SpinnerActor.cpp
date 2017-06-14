#include "stdafx.h"
#include "SpinnerActor.h"
#include "SceneNode.h"
#include <exception>


namespace GLEngine
{
	SpinnerActor::SpinnerActor()
	{
	}

	SpinnerActor::SpinnerActor(Quaternion rotation)
		: Actor(), _rotation(rotation)
	{
	}

	SpinnerActor::~SpinnerActor()
	{
	}

	void SpinnerActor::Increment(float deltaTime)
	{
		SceneNode* parentNode = GetParentNode();

		if (parentNode != nullptr)
		{
			Matrix4* parentRelative = parentNode->GetRelativeTransformation();

			Matrix4 rotationMatrix = Matrix4::CreateRotationMatrixFromQuaternion(Quaternion(_rotation.GetX(), _rotation.GetY(), _rotation.GetZ(), _rotation.GetW() * deltaTime));
			//Matrix4 rotationMatrix = Matrix4::CreateRotationY(1.0f);

			*parentRelative = *parentRelative * rotationMatrix;
		}
		else
		{
			throw new std::exception("Spinner actor does not have a parent SceneNode.");
		}
	}
}
