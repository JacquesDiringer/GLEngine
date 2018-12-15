#pragma once

#include <SceneGraph\SceneNode.h>
#include <Math\Vector3.h>

#include <list>

using std::list;

namespace GLEngine
{

	// Represents an axis aligned cube that can contain sub elements.
	// A sub element can be contained by several cells. This is where the system differs from bounding spheres.
	// A cell will always have the same position and size.
	class Cell
		: public SceneElement
	{
	public:

		// Visitable element structure.
		struct VisitableNode
		{
			short lastFrameVisited;
			SceneNode* node;

			VisitableNode(SceneNode* nodePtr)
			{
				lastFrameVisited = 0;
				node = nodePtr;
			}
		};

		// Cell class.

		Cell();
		Cell(const GLEngineMath::Vector3& position, float size);
		~Cell();

		virtual void Accept(SceneElementVisitor* visitor);

		void AddSubNode(VisitableNode * visitableNode);
		void DeleteSubNode(VisitableNode * visitableNode);
		list<VisitableNode*>& GetSubNodes() { return _subNodes; }
		bool IsEmpty() const { return _subNodes.size() == 0; }

		const GLEngineMath::Vector3& GetPosition() const { return _position; }
		float GetSize() const { return _size; }

	private:
		list<VisitableNode*> _subNodes;
		GLEngineMath::Vector3 _position;
		float _size;
	};
}