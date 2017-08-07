#include "stdafx.h"
#include "ThirdViewOrientationActor.h"
#include "Matrix4.h"

using Math::Matrix4;

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
			Matrix4* parentRelative = parentNode->GetRelativeTransformation();

		}
	}
}