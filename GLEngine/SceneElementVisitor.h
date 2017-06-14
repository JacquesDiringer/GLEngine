#pragma once

namespace GLEngine
{
	class SceneNode;
	class Model;

	class SceneElementVisitor
	{
	public:
		SceneElementVisitor();
		~SceneElementVisitor();

		// Visitor pattern.
		virtual void Visit(SceneNode* sceneNodeToVisit) = 0;
		virtual void Visit(Model* modelToVisit) = 0;
	};
}