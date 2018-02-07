#pragma once

namespace GLEngine
{
	class SceneElementVisitor;
	class SceneNode;

	class SceneElement
	{
	public:
		SceneElement();
		~SceneElement();

		virtual void Accept(SceneElementVisitor* visitor) = 0;

		SceneNode* GetParentNode() const { return _parentNode; }
		void SetParentNode(SceneNode* parent) { _parentNode = parent; }

		virtual void SetIsUpToDate(const bool value);

		void RemoveFromParentNode();

	private:
		SceneNode* _parentNode = nullptr;
	};
}