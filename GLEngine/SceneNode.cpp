#include "stdafx.h"
#include "SceneNode.h"

#include <typeinfo>


namespace GLEngine
{
	SceneNode::SceneNode()
		: SceneElement()
	{
		_relativeTransformation = new Matrix4();
		_worldTransformation = new Matrix4();
		SetWorldMatrixIsUpToDate(false);
		_subElements = list<SceneElement*>();
	}


	SceneNode::~SceneNode()
	{
	}

	void SceneNode::SetRelativeTransformation(Matrix4 newMatrix)
	{
		*_relativeTransformation = newMatrix;
		SetWorldMatrixIsUpToDate(false);
	}

	void SceneNode::SetRelativeTransformation(Matrix4 * newMatrix)
	{
		_relativeTransformation = newMatrix;
		SetWorldMatrixIsUpToDate(false);
	}

	Matrix4 * SceneNode::GetWorldTransformation()
	{
		if (!_worldMatrixIsUpToDate)
		{
			if (GetParentNode() == nullptr)
			{
				*_worldTransformation = *_relativeTransformation;
			}
			else
			{
				*_worldTransformation = Matrix4::Multiply(*(GetParentNode()->GetWorldTransformation()), *_relativeTransformation);
			}
		}

		SetWorldMatrixIsUpToDate(true);

		return _worldTransformation;
	}

	void SceneNode::AddSubElement(SceneElement * element)
	{
		// Add the new element to the sub elements list.
		_subElements.push_front(element);

		// Set the parent node of the new element so that it point at this instance.
		element->SetParentNode(this);
	}

	void SceneNode::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void SceneNode::SetWorldMatrixIsUpToDate(const bool value)
	{
		_worldMatrixIsUpToDate = value;

		// If false, invalidate sub scene nodes.
		if (!value)
		{
			for each (SceneElement* currentElement in _subElements)
			{
				SceneNode* currentSceneNode = dynamic_cast<SceneNode*>(currentElement);

				if (currentSceneNode != nullptr)
				{
					currentSceneNode->SetWorldMatrixIsUpToDate(false);
				}
			}
		}
	}
	SceneNode * SceneNode::CreateChild()
	{
		SceneNode* child = new SceneNode();

		AddSubElement(child);

		return child;
	}
}