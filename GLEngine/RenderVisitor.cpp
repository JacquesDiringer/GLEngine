#include "stdafx.h"
#include "RenderVisitor.h"
#include "SceneElement.h"
#include "SceneNode.h"
#include "Actor.h"
#include "Renderable.h"


namespace GLEngine
{
	RenderVisitor::RenderVisitor()
		: SceneElementVisitor()
	{
	}

	RenderVisitor::RenderVisitor(SceneManager * sceneManager)
		: SceneElementVisitor(), _sceneManager(sceneManager)
	{
	}

	RenderVisitor::~RenderVisitor()
	{
	}

	void RenderVisitor::Visit(SceneNode * sceneNodeToVisit)
	{
		// Visit the sub elements.
		for each (SceneElement* currentSubElement in sceneNodeToVisit->GetSubElements())
		{
			currentSubElement->Accept(this);
		}
	}

	void RenderVisitor::Visit(Renderable * renderableToVisit)
	{
		renderableToVisit->Render(_sceneManager);
	}

	void RenderVisitor::Visit(Actor * actorToVisit)
	{
	}

}