#include "stdafx.h"
#include "ThirdViewOrientationActor.h"
#include "Matrix4.h"
#include "SceneNode.h"
#include "Vector3.h"

using Math::Matrix4;
using Math::Vector3;

namespace GLEngine
{
	ThirdViewOrientationActor::ThirdViewOrientationActor()
	{
	}

	ThirdViewOrientationActor::ThirdViewOrientationActor(SceneNode * target)
		: _target(target)
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
			Vector3 parentRelativePosition = parentNode->GetRelativeTransformation()->Position();

			parentNode->get
		}
	}
}