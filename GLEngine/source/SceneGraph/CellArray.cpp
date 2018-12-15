#include "stdafx.h"
#include <SceneGraph\CellArray.h>
#include <SceneGraph\SceneElementVisitor.h>
#include <SceneGraph\SceneNode.h>
#include <cmath>
#include <iostream>

namespace GLEngine
{
	CellArray::CellArray()
		: _cellsSize(0.0f)
	{
		_subCells = std::map<GLEngineMath::Vector3, Cell*>();
		_visitableNodes = std::list<Cell::VisitableNode*>();
	}

	CellArray::CellArray(float cellsSize)
		: _cellsSize(cellsSize)
	{
		_subCells = std::map<GLEngineMath::Vector3, Cell*>();
		_visitableNodes = std::list<Cell::VisitableNode*>();
	}


	CellArray::~CellArray()
	{
	}

	void CellArray::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	inline float CellArray::ClosestCellCoordinate(float initialCoordinate) const
	{
		return std::roundf(initialCoordinate / _cellsSize) * _cellsSize;
	}

	void CellArray::AddSceneNode(SceneNode * node)
	{
		// Check that the node doesn't have a parent already.
		// Then set it as the child of the cell array.
		if (node->GetParent() == nullptr)
		{
			node->SetParent(this);
		}
		else
		{
			throw new std::exception("A scene node added to a cell array should not have a parent already.");
		}

		// Determine which cells this scene node intersects.
		GLEngineMath::Vector3 nodePosition = node->GetWorldTransformation().Position();
		float boundingSphereRadius = node->GetBoundingSphereRadius();
		float xMinCell = ClosestCellCoordinate(nodePosition.X() - boundingSphereRadius);
		float xMaxCell = ClosestCellCoordinate(nodePosition.X() + boundingSphereRadius);
		float yMinCell = ClosestCellCoordinate(nodePosition.Y() - boundingSphereRadius);
		float yMaxCell = ClosestCellCoordinate(nodePosition.Y() + boundingSphereRadius);
		float zMinCell = ClosestCellCoordinate(nodePosition.Z() - boundingSphereRadius);
		float zMaxCell = ClosestCellCoordinate(nodePosition.Z() + boundingSphereRadius);

		// Create the VisitableNode instance that is going to be shared accross Cells.
		Cell::VisitableNode* newVisitableNode = new Cell::VisitableNode(node);
		_visitableNodes.push_back(newVisitableNode);

		// Browse the cells and determinne whether it exists or not.
		for (float currentXCell = xMinCell; currentXCell <= xMaxCell; currentXCell += _cellsSize)
		{
			for (float currentYCell = yMinCell; currentYCell <= yMaxCell; currentYCell += _cellsSize)
			{
				for (float currentZCell = zMinCell; currentZCell <= zMaxCell; currentZCell += _cellsSize)
				{
					// Look for the current cell in the map.
					GLEngineMath::Vector3 position = GLEngineMath::Vector3(currentXCell, currentYCell, currentZCell);
					std::map<GLEngineMath::Vector3, Cell*>::iterator it = _subCells.find(position);

					Cell * selectedCell;

					if (it != _subCells.end())
					{
						// If it exists, select the cell to have the node added to it.
						selectedCell = it->second;
					}
					else
					{
						// If it doesn't exist, create the cell and add it to the map.
						// Then select it.
						selectedCell = AddSubCell(position);
					}

					// Finally add the node to the sub cell.
					selectedCell->AddSubNode(newVisitableNode);
				}
			}
		}
	}

	void CellArray::DeleteSceneNode(SceneNode * node)
	{
		// Determine which cells this scene node intersects.
		GLEngineMath::Vector3 nodePosition = node->GetWorldTransformation().Position();
		float boundingSphereRadius = node->GetBoundingSphereRadius();
		float xMinCell = ClosestCellCoordinate(nodePosition.X() - boundingSphereRadius);
		float xMaxCell = ClosestCellCoordinate(nodePosition.X() + boundingSphereRadius);
		float yMinCell = ClosestCellCoordinate(nodePosition.Y() - boundingSphereRadius);
		float yMaxCell = ClosestCellCoordinate(nodePosition.Y() + boundingSphereRadius);
		float zMinCell = ClosestCellCoordinate(nodePosition.Z() - boundingSphereRadius);
		float zMaxCell = ClosestCellCoordinate(nodePosition.Z() + boundingSphereRadius);

		// Browse the cells and determinne whether it exists or not.
		for (float currentXCell = xMinCell; currentXCell <= xMaxCell; currentXCell += _cellsSize)
		{
			for (float currentYCell = yMinCell; currentYCell <= yMaxCell; currentYCell += _cellsSize)
			{
				for (float currentZCell = zMinCell; currentZCell <= zMaxCell; currentZCell += _cellsSize)
				{
					// Look for the current cell in the map.
					GLEngineMath::Vector3 position = GLEngineMath::Vector3(currentXCell, currentYCell, currentZCell);
					std::map<GLEngineMath::Vector3, Cell*>::iterator it = _subCells.find(position);

					if (it != _subCells.end())
					{
						// If it exists, delete the node from it.
						Cell * foundCell = it->second;

						foundCell->DeleteSubNode(FindCorrespondingNode(node));

						if (foundCell->IsEmpty())
						{
							DeleteSubCell(it);
						}
					}
					else
					{
						// If it doesn't exist, do nothing.
						// Or throw an exception ?
					}
				}
			}
		}
	}

	void CellArray::RemoveChild(SceneElement * elementToRemove)
	{
		SceneNode * node = dynamic_cast<SceneNode*>(elementToRemove);

		if (node != nullptr)
		{
			DeleteSceneNode(node);
		}
	}

	Cell * CellArray::AddSubCell(const GLEngineMath::Vector3& cellPosition)
	{
		Cell * newCell = new Cell(cellPosition, _cellsSize);
		_subCells.insert(std::pair<GLEngineMath::Vector3, Cell*>(cellPosition, newCell));

		return newCell;
	}

	void CellArray::DeleteSubCell(std::map<GLEngineMath::Vector3, Cell*>::iterator it)
	{
		Cell * cellToDelete = it->second;

		// First remove the cell from the map.
		_subCells.erase(it);

		// Then delete it from memory.
		// This should be optimized with a pooling system.
		delete cellToDelete;
	}

	Cell::VisitableNode * CellArray::FindCorrespondingNode(SceneNode * node) const
	{
		Cell::VisitableNode* result = nullptr;

		for (auto currentVisitable : _visitableNodes)
		{
			if (currentVisitable->node == node)
			{
				result = currentVisitable;
			}
		}

		if (result == nullptr)
		{
			throw new std::exception("Corresponding visitable node could not be found.");
		}

		return result;
	}

}