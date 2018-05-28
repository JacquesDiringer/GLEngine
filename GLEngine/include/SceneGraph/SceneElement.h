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

		SceneNode* GetParentNode() const { return _parentNode; }
		void SetParentNode(SceneNode* parent) { _parentNode = parent; }

		// Position relative to parent. Should be a null vector excepting for SceneNodes.
		virtual const GLEngineMath::Vector3 GetRelativePosition() const;

		virtual float GetBoundingSphereRadius();

		bool GetIsBoundingUpToDate() const { return _isBoundingUpToDate; }
		virtual void SetIsBoundingUpToDate(bool value);

		virtual void UpdateBoundings();

		virtual void SetIsUpToDate(const bool value);

		void RemoveFromParentNode();

	protected:
		bool _isBoundingUpToDate = false;

	private:
		SceneNode* _parentNode = nullptr;
	};
}