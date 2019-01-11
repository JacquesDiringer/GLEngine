#include <stdafx.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneNode.h>


namespace GLEngine
{
	SceneElement::SceneElement()
	{
		_parent = nullptr;
	}


	SceneElement::~SceneElement()
	{
		RemoveFromParentNode();
	}

	SceneNode * SceneElement::GetParentNode() const
	{
		if (_parentNode == nullptr)
		{
			_parentNode = dynamic_cast<SceneNode*>(GetParent());
		}

		return _parentNode;
	}

	const GLEngineMath::Vector3 SceneElement::GetRelativePosition() const
	{
		return GLEngineMath::Vector3();
	}

	float SceneElement::GetBoundingSphereRadius()
	{
		if (!_isBoundingUpToDate)
		{
			UpdateBoundings();
		}

		return 0.0f;
	}

	void SceneElement::SetIsBoundingUpToDate(bool value)
	{
		_isBoundingUpToDate = value;
	}

	void SceneElement::UpdateBoundings()
	{
		_isBoundingUpToDate = true;
	}

	void SceneElement::SetIsUpToDate(const bool value)
	{
		// Void
	}

	void SceneElement::RemoveFromParentNode()
	{
		if (_parent != nullptr)
		{
			// Go in the parent and delete this child from the children vector.
			_parent->RemoveChild(this);

			// Delete the link to the parent
			_parent = nullptr;
		}
	}

	void SceneElement::RemoveChild(SceneElement * elementToRemove)
	{
		// Void
	}
}