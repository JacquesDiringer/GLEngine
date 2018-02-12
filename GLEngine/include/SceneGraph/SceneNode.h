#pragma once

#include<vector>

#include <Math\Matrix4.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneElementVisitor.h>

using std::vector;
using Math::Matrix4;

namespace GLEngine
{
	class SceneNode
		: public SceneElement
	{
	public:
		SceneNode();
		~SceneNode();

		const Matrix4& GetRelativeTransformation()const { return _relativeTransformation; }
		void SetRelativeTransformation(const Matrix4& newMatrix);

		const Matrix4& GetWorldTransformation();

		vector<SceneElement*>* GetSubElements() { return &_subElements; }

		// Adds children.
		void AddSubElement(SceneElement* element);

		// Part of the visitor pattern.
		virtual void Accept(SceneElementVisitor* visitor);

		// Sets the fact that world matrix is up to date.
		virtual void SetIsUpToDate(const bool value);

		// Adds a SceneNode child and links the current node as it's parent.
		SceneNode* CreateChild();

		// Remove a sub element from the children.
		// This function should only be used from a SceneElement.
		void RemoveChild(SceneElement* elementToRemove);

	private:
		Matrix4 _relativeTransformation;
		Matrix4 _worldTransformation;
		bool _worldMatrixIsUpToDate;
		vector<SceneElement*> _subElements;
	};
}