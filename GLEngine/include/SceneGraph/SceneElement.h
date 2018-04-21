#pragma once

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

		virtual void SetIsUpToDate(const bool value);

		void RemoveFromParentNode();

	private:
		SceneNode* _parentNode = nullptr;
	};
}