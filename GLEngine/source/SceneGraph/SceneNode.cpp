#include <stdafx.h>
#include <SceneGraph\SceneNode.h>

namespace GLEngine
{
	SceneNode::SceneNode()
		: SceneElement()
	{
		_relativeTransformation = GLEngineMath::Matrix4::Identity();
		_worldTransformation = GLEngineMath::Matrix4::Identity();
		SetIsUpToDate(false);
		_subElements = vector<SceneElement*>();
	}


	SceneNode::~SceneNode()
	{
		// Delete all children.
		// This will also remove them from the children vector, while removing the link to the parent at the same time.
		vector<SceneElement*> childrenToDelete = vector<SceneElement*>(_subElements);
		for each (SceneElement* currentChild in childrenToDelete)
		{
			delete(currentChild);
		}
	}

	void SceneNode::SetRelativeTransformation(const GLEngineMath::Matrix4& newMatrix)
	{
		_relativeTransformation = newMatrix;
		SetIsUpToDate(false);
		SetIsBoundingUpToDate(false);
	}

	const GLEngineMath::Matrix4& SceneNode::GetWorldTransformation()
	{
		if (!_worldMatrixIsUpToDate)
		{
			if (GetParentNode() == nullptr)
			{
				_worldTransformation = _relativeTransformation;
			}
			else
			{
				_worldTransformation = GLEngineMath::Matrix4::Multiply((GetParentNode()->GetWorldTransformation()), _relativeTransformation);
			}
		}

		SetIsUpToDate(true);

		return _worldTransformation;
	}

	const GLEngineMath::Vector3 SceneNode::GetRelativePosition() const
	{
		return _relativeTransformation.Position();
	}

	float SceneNode::GetBoundingSphereRadius()
	{
		if (!_isBoundingUpToDate)
		{
			UpdateBoundings();
		}

		return _boundingSphereRadius;
	}

	void SceneNode::SetIsBoundingUpToDate(bool value)
	{
		if (value != _isBoundingUpToDate)
		{
			if (!value)
			{
				SceneNode* parentNode = GetParentNode();
				if (parentNode != nullptr)
				{
					parentNode->SetIsBoundingUpToDate(value);
				}
			}

			_isBoundingUpToDate = value;
		}
	}

	void SceneNode::UpdateBoundings()
	{
		for (SceneElement* currentElement : _subElements)
		{
			// The radius of the bounding sphere of the scene node will correspond to the minimum radius necessary to contain all sub element's bounding spheres.
			float currentElementRequiredRadius = currentElement->GetRelativePosition().Length() + currentElement->GetBoundingSphereRadius();
			if (currentElementRequiredRadius > _boundingSphereRadius)
			{
				_boundingSphereRadius = currentElementRequiredRadius;
			}
		}

		// Set the up to date boolean to true.
		SetIsBoundingUpToDate(true);
	}

	void SceneNode::AddSubElement(SceneElement * element)
	{
		// Add the new element to the sub elements list.
		_subElements.push_back(element);

		// Set the parent node of the new element so that it point at this instance.
		element->SetParentNode(this);
	}

	void SceneNode::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void SceneNode::SetIsUpToDate(const bool value)
	{
		_worldMatrixIsUpToDate = value;

		// If false, invalidate sub scene nodes.
		if (!value)
		{
			for each (SceneElement* currentElement in _subElements)
			{
				currentElement->SetIsUpToDate(value);
			}
		}
	}

	SceneNode * SceneNode::CreateChild()
	{
		SceneNode* child = new SceneNode();

		AddSubElement(child);

		return child;
	}

	void SceneNode::RemoveChild(SceneElement * elementToRemove)
	{
		// Find the child in the children vector and remove it.
		vector<SceneElement*>::iterator findIterator = std::find(_subElements.begin(), _subElements.end(), elementToRemove);
		_subElements.erase(findIterator);
	}
}