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
		_subNodes.remove(visitableNode);
	}
}