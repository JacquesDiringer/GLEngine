#pragma once

namespace GLEngine
{
	class SceneNode;

	class SceneElementVisitor
	{
	public:
		SceneElementVisitor();
		~SceneElementVisitor();

		// Visitor pattern.
		virtual void Visit(SceneNode* sceneNodeToVisit) = 0;
	};
}