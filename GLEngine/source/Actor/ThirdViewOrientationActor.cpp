#include <stdafx.h>
#include <Actor\ThirdViewOrientationActor.h>
#include <Math\Matrix4.h>
#include <SceneGraph\SceneNode.h>
#include <Math\Vector3.h>

using GLEngineMath::Matrix4;
using GLEngineMath::Vector3;

namespace GLEngine
{
	ThirdViewOrientationActor::ThirdViewOrientationActor(SceneManager& sceneManager)
		: Actor(sceneManager)
	{
	}

	ThirdViewOrientationActor::ThirdViewOrientationActor(SceneManager& sceneManager, SceneNode * target)
		: Actor(sceneManager), _target(target)
	{
	}

	ThirdViewOrientationActor::~ThirdViewOrientationActor()
	{
	}

	void ThirdViewOrientationActor::Increment(float deltaTime)
	{
		SceneNode* parentNode = GetParentNode();

		if (parentNode != nullptr)
		{
			Matrix4 relativeTransformation = Matrix4::CreateTargetPositionCameraYAxis(parentNode->GetWorldTransformation().Position(), _target->GetWorldTransformation().Position());
			// We invert the matrix because the algorithm that constructed it is actually the one used to compute the view matrix of the camera (this later leads to an other inversion, to recover the view matrix, this silly non optimized double computation needs to be fixed).
			relativeTransformation.InvertRT();

			SceneNode* grandParentNode = parentNode->GetParentNode();
			Matrix4 grandParentWorldMatrix;
			if (grandParentNode != nullptr)
			{
				grandParentWorldMatrix = grandParentNode->GetWorldTransformation();
			}
			else
			{
				grandParentWorldMatrix = Matrix4::Identity();
			}

			grandParentWorldMatrix.InvertRT();

			// The relative matrix needs to counter the rotation and translation of the above matrices, in order to have the node facing the right direction in world space.
			// Therefore we multiply it by the inverse transformation of the parent scene node.
			relativeTransformation = grandParentWorldMatrix * relativeTransformation;

			parentNode->SetRelativeTransformation(relativeTransformation);
		}
	}
}