#include "stdafx.h"
#include <SceneGraph\Cell.h>
#include <SceneGraph\SceneElementVisitor.h>


namespace GLEngine
{
	Cell::Cell()
	{
		// Invalid default constructor.
		_position = GLEngineMath::Vector3();
		_size = 0.0f;
	}

	Cell::Cell(const GLEngineMath::Vector3 & position, float size)
		: _position(position), _size(size)
	{
	}


	Cell::~Cell()
	{
	}

	void Cell::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void Cell::AddSubNode(VisitableNode * visitableNode)
	{
		_subNodes.push_back(visitableNode);
	}

	void Cell::DeleteSubNode(VisitableNode * visitableNode)
	{
		//list<VisitableNode*>::iterator visitableNodeFinder = _subNodes.begin();
		//while (visitableNodeFinder != _subNodes.end() && (*visitableNodeFinder)->node != node)
		//{
		//	visitableNodeFinder++;
		//}

		//if (visitableNodeFinder == _subNodes.end())
		//{
		//	throw new std::exception("The node to delete should have been found in the cell.");
		//}

		//VisitableNode * visitableToBeDeleted = *visitableNodeFinder;

		//// Erase the node from the cell's list.
		//_subNodes.erase(visitableNodeFinder);

		//// Delete the visitable node but let the node live
		//delete visitableToBeDeleted;

		_subNodes.remove(visitableNode);
	}

	void Cell::ResetVisitedElements()
	{
		for each (VisitableNode* currentElement in _subNodes)
		{
			currentElement->visited = false;
		}
	}
}