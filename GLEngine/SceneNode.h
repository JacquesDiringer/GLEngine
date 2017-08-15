#pragma once

#include<list>

#include "Matrix4.h"
#include"SceneElement.h"
#include"SceneElementVisitor.h"

using std::list;
using Math::Matrix4;

namespace GLEngine
{
	class SceneNode
		: public SceneElement
	{
	public:
		SceneNode();
		~SceneNode();

		Matrix4* GetRelativeTransformation()const { return _relativeTransformation; }
		void SetRelativeTransformation(Matrix4 newMatrix);
		void SetRelativeTransformation(Matrix4* newMatrix);

		Matrix4* GetWorldTransformation();

		list<SceneElement*> GetSubElements()const { return _subElements; }

		// Adds children.
		void AddSubElement(SceneElement* element);

		// Part of the visitor pattern.
		virtual void Accept(SceneElementVisitor* visitor);

		// Sets the fact that world matrix is up to date.
		virtual void SetIsUpToDate(const bool value);

		SceneNode* CreateChild();


	private:
		Matrix4* _relativeTransformation;
		Matrix4* _worldTransformation;
		bool _worldMatrixIsUpToDate;
		list<SceneElement*> _subElements;
	};
}