#include "stdafx.h"
#include "RenderVisitor.h"
#include "SceneElement.h"
#include "SceneNode.h"
#include "Actor.h"
#include "Model.h"
#include "EnvironmentMapSky.h"


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

	void RenderVisitor::Visit(Model * modelToVisit)
	{
		modelToVisit->Render(_sceneManager);
	}

	void RenderVisitor::Visit(EnvironmentMapSky * skyToVisit)
	{
		skyToVisit->Render(_sceneManager);
	}

	void RenderVisitor::Visit(Actor * actorToVisit)
	{
		// Void
	}
}