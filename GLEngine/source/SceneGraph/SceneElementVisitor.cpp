#include <stdafx.h>
#include <SceneGraph\SceneElementVisitor.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneNode.h>


namespace GLEngine
{
	SceneElementVisitor::SceneElementVisitor()
	{
	}


	SceneElementVisitor::~SceneElementVisitor()
	{
	}

	void SceneElementVisitor::Visit(SceneNode * sceneNodeToVisit)
	{
		// Visit the sub elements.
		for each (SceneElement* currentSubElement in *sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}
}