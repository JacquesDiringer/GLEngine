#pragma once
#include "SceneElementVisitor.h"
namespace GLEngine
{
	class UpdateVisitor :
		public SceneElementVisitor
	{
	public:
		UpdateVisitor();
		~UpdateVisitor();

		// Visitor pattern.
		virtual void Visit(SceneNode* sceneNodeToVisit);
		virtual void Visit(Model* modelToVisit);
	};
}

