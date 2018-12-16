#pragma once

#include <SceneGraph\SceneElement.h>
#include <SceneGraph\Cell.h>

#include <map>
#include <unordered_map>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API CellArray
		: public SceneElement
	{
	public:
		CellArray();
		CellArray(float cellsSize);
		~CellArray();

		// Part of the visitor pattern.
		virtual void Accept(SceneElementVisitor* visitor);

		std::map<GLEngineMath::Vector3, Cell*>& GetSubCells() { return _subCells; }

		// Returns the position of the closest cell.
		inline float ClosestCellCoordinate(float initialCoordinate) const;

		// Adds a scene node as a child of the cell array.
		void AddSceneNode(SceneNode * node);
		void DeleteSceneNode(SceneNode * node);

		// Remove a sub element from the children.
		// This function should only be used from a SceneElement.
		virtual void RemoveChild(SceneElement* elementToRemove);

	private:
		Cell * AddSubCell(const GLEngineMath::Vector3& cellPosition);
		void DeleteSubCell(std::map<GLEngineMath::Vector3, Cell*>::iterator it);

		Cell::VisitableNode* FindCorrespondingNode(SceneNode * node) const;

	private:
		// A list of sub cells, the center of the central cell is at (0, 0, 0) rather than (size/2, size/2, size/2). The makes it quicker to find the closest cell center to a coordinate.
		// The key is the cell's position.
		std::map<GLEngineMath::Vector3, Cell*> _subCells;

		// Size of the sub cells.
		float _cellsSize;

		// An unordered map (hash table) of the visitable nodes stored in the cells.
		// Count should be equal to the number of SceneNodes stored in the CellArray.
		std::unordered_map<SceneNode*, Cell::VisitableNode*> _visitableNodesMap;
	};
}