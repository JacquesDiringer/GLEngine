#pragma once

#include <Math\Vector3.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class SceneElementVisitor;
	class SceneNode;

	class GLENGINE_API SceneElement
	{
	public:
		SceneElement();
		virtual ~SceneElement();

		virtual void Accept(SceneElementVisitor* visitor) = 0;

		SceneElement* GetParent() const { return _parent; }
		void SetParent(SceneElement* parent) { _parent = parent; }

		// Ensures that the dynamic_cast to get the parent node is done only once.
		virtual SceneNode* GetParentNode() const;

		// Position relative to parent. Should be a null vector excepting for SceneNodes.
		virtual const GLEngineMath::Vector3 GetRelativePosition() const;

		virtual float GetBoundingSphereRadius();

		bool GetIsBoundingUpToDate() const { return _isBoundingUpToDate; }
		virtual void SetIsBoundingUpToDate(bool value);

		virtual void UpdateBoundings();

		virtual void SetIsUpToDate(const bool value);

		void RemoveFromParentNode();

		// Remove a sub element from the children.
		// This function should only be used from a SceneElement.
		virtual void RemoveChild(SceneElement* elementToRemove);

	protected:
		bool _isBoundingUpToDate = false;
		mutable SceneNode* _parentNode = nullptr;

	private:
		SceneElement * _parent = nullptr;
	};
}