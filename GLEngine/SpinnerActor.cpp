#include "stdafx.h"
#include "SpinnerActor.h"
#include "SceneNode.h"
#include <exception>


namespace GLEngine
{
	SpinnerActor::SpinnerActor()
	{
	}

	SpinnerActor::SpinnerActor(Quaternion* quaternion)
		: Actor(), _quaternion(quaternion), _rotationMatrix(nullptr), _quaternionConstructor(true)
	{
	}

	SpinnerActor::SpinnerActor(Matrix4 * rotationMatrix)
		: Actor(), _quaternion(nullptr), _rotationMatrix(rotationMatrix), _quaternionConstructor(false)
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

			Matrix4 rotationMatrix;
			
			if (_quaternionConstructor)
			{
				rotationMatrix = Matrix4::CreateRotationMatrixFromQuaternion(Quaternion(_quaternion->GetX(), _quaternion->GetY(), _quaternion->GetZ(), _quaternion->GetW() * deltaTime));
			}
			else
			{
				rotationMatrix = *_rotationMatrix;

				// TODO: Make it work with the delta time.
				//rotationMatrix = *_rotationMatrix  * deltaTime;
			}

			parentNode->SetRelativeTransformation(*parentRelative * rotationMatrix);
		}
		else
		{
			throw new std::exception("Spinner actor does not have a parent SceneNode.");
		}
	}
}
