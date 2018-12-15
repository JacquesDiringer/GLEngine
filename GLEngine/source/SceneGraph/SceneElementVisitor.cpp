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
		for each (SceneElement* currentSubElement in *sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}

	void SceneElementVisitor::Visit(CellArray * cellArrayToVisit)
	{
		// Before starting to visit, mark all sub elements from all cells as not visited.
		cellArrayToVisit->ResetVisitedCells();

		//std::cout << "Cell array size : " << cellArrayToVisit->GetSubCells().size() << std::endl;

		// Then visit them
		for (auto currentPair : cellArrayToVisit->GetSubCells())
		{
			currentPair.second->Accept(this);
		}
	}

	void SceneElementVisitor::Visit(Cell * cellToVisit)
	{
		for each (Cell::VisitableNode* currentElement in cellToVisit->GetSubNodes())
		{
			// Visit only if it has not already been visited by another cell.
			if (!currentElement->visited)
			{
				currentElement->node->Accept(this);

				// Mark the element as visited.
				currentElement->visited = true;
			}
		}
	}
}