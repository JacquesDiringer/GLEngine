#pragma once

namespace GLEngine
{
	class SceneNode;
	class Actor;
	class Renderable;

	class SceneElementVisitor
	{
	public:
		SceneElementVisitor();
		~SceneElementVisitor();

		// Visitor pattern.
		virtual void Visit(SceneNode* sceneNodeToVisit) = 0;
		virtual void Visit(Renderable* renderableToVisit) = 0;
		virtual void Visit(Actor* actorToVisit) = 0;
	};
}