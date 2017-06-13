#include "stdafx.h"
#include "UpdateVisitor.h"
#include "SceneElement.h"
#include "SceneNode.h"

namespace GLEngine
{
	UpdateVisitor::UpdateVisitor()
	{
	}

	UpdateVisitor::~UpdateVisitor()
	{
	}

	void UpdateVisitor::Visit(SceneNode * sceneNodeToVisit)
	{
		// Visit the sub elements.
		for each (SceneElement* currentSubElement in sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}
}