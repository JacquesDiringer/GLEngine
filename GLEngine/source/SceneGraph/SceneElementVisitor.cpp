#include <stdafx.h>
#include <SceneGraph\SceneElementVisitor.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneNode.h>
#include <SceneGraph\Cell.h>
#include <SceneGraph\CellArray.h>

#include <iostream>

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
		for (SceneElement* currentSubElement : *sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}

	void SceneElementVisitor::Visit(CellArray * cellArrayToVisit)
	{
		// Then visit them
		for (auto currentPair : cellArrayToVisit->GetSubCells())
		{
			currentPair.second->Accept(this);
		}
	}

	void SceneElementVisitor::Visit(Cell * cellToVisit)
	{
		for (Cell::VisitableNode* currentElement : cellToVisit->GetSubNodes())
		{

			// Visit only if it has not already been visited by another cell.
			if (currentElement->lastFrameVisited != _currentFrameId)
			{
				currentElement->node->Accept(this);

				// Mark the element as visited.
				currentElement->lastFrameVisited = _currentFrameId;
			}
		}
	}

	void SceneElementVisitor::IncrementCurrentFrameId()
	{
		_currentFrameId++;

		// Make sure we never reach the maximum value of a short.
		if (_currentFrameId > 1000)
		{
			_currentFrameId = 0;
		}
	}
}