#pragma once

#include<vector>

#include <Math\Matrix4.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneElementVisitor.h>

using std::vector;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API SceneNode
		: public SceneElement
	{
	public:
		SceneNode();
		~SceneNode();

		const GLEngineMath::Matrix4& GetRelativeTransformation()const { return _relativeTransformation; }
		void SetRelativeTransformation(const GLEngineMath::Matrix4& newMatrix);

		const GLEngineMath::Matrix4& GetWorldTransformation();

		// Position relative to parent.
		virtual const GLEngineMath::Vector3 GetRelativePosition() const;

		virtual float GetBoundingSphereRadius();

		virtual void SetIsBoundingUpToDate(bool value);

		virtual void UpdateBoundings();

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
		virtual void RemoveChild(SceneElement* elementToRemove);

		// Ensures that the dynamic_cast to get the parent node is done only once.
		SceneNode* GetParentNode() const;

	private:
		GLEngineMath::Matrix4 _relativeTransformation;
		GLEngineMath::Matrix4 _worldTransformation;
		bool _worldMatrixIsUpToDate;
		vector<SceneElement*> _subElements;
		float _boundingSphereRadius = 0;

		mutable bool _isRootNode = false;
	};
}