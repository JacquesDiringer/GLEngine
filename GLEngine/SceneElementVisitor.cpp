#include "stdafx.h"
#include "SceneElementVisitor.h"
#include "SceneElement.h"
#include "SceneNode.h"


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
		for each (SceneElement* currentSubElement in sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}
}