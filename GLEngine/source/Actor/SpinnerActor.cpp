#include <stdafx.h>
#include <Actor\SpinnerActor.h>
#include <SceneGraph\SceneNode.h>
#include <exception>
#include <iostream>
#include <Math\Vector3.h>

namespace GLEngine
{
	SpinnerActor::SpinnerActor(SceneManager& sceneManager)
		: Actor(sceneManager)
	{
	}

	SpinnerActor::SpinnerActor(SceneManager& sceneManager, const Quaternion& quaternion)
		: Actor(sceneManager), _quaternion(quaternion), _quaternionConstructor(true)
	{
	}

	SpinnerActor::SpinnerActor(SceneManager& sceneManager, const Matrix4& rotationMatrix)
		: Actor(sceneManager), _rotationMatrix(rotationMatrix), _quaternionConstructor(false)
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
			Matrix4 parentRelative = parentNode->GetRelativeTransformation();

			Matrix4 rotationMatrix;
			
			if (_quaternionConstructor)
			{
				rotationMatrix = Matrix4::CreateRotationMatrixFromQuaternion(Quaternion(_quaternion.GetX(), _quaternion.GetY(), _quaternion.GetZ(), _quaternion.GetW() * deltaTime));
			}
			else
			{
				rotationMatrix = _rotationMatrix;

				// TODO: Make it work with the delta time.
				//rotationMatrix = *_rotationMatrix  * deltaTime;
			}

			parentNode->SetRelativeTransformation(parentRelative * rotationMatrix);
		}
		else
		{
			throw new std::exception("Spinner actor does not have a parent SceneNode.");
		}
	}
}
