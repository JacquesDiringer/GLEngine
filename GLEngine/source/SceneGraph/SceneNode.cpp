#include <stdafx.h>
#include <SceneGraph\SceneNode.h>

namespace GLEngine
{
	SceneNode::SceneNode()
		: SceneElement()
	{
		_relativeTransformation = new Matrix4();
		_worldTransformation = new Matrix4();
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

		// Delete the matrices on the heap.
		delete(_relativeTransformation);
		delete(_worldTransformation);
	}

	void SceneNode::SetRelativeTransformation(Matrix4 newMatrix)
	{
		*_relativeTransformation = newMatrix;
		SetIsUpToDate(false);
	}

	void SceneNode::SetRelativeTransformation(Matrix4 * newMatrix)
	{
		_relativeTransformation = newMatrix;
		SetIsUpToDate(false);
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

		SetIsUpToDate(true);

		return _worldTransformation;
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