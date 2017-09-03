#include "stdafx.h"
#include "UpdateVisitor.h"
#include "SceneElement.h"
#include "SceneNode.h"
#include "Actor.h"
#include "Model.h"
#include "EnvironmentMapSky.h"

// GLFW
#include <GLFW/glfw3.h>

namespace GLEngine
{
	UpdateVisitor::UpdateVisitor()
		: _timeSinceLastUpdate(0), _timeAtLastUpdate(0)
	{
	}

	UpdateVisitor::~UpdateVisitor()
	{
	}

	void UpdateVisitor::Visit(Model * modelToVisit)
	{
		// Void
	}

	void UpdateVisitor::Visit(EnvironmentMapSky * skyToVisit)
	{
		// Void
	}

	void UpdateVisitor::Visit(PointLight * pointLightToVisit)
	{
		// Void
	}

	void UpdateVisitor::Visit(Actor * actorToVisit)
	{
		actorToVisit->Increment(GetTimeSinceLastUpdate());
	}

	void UpdateVisitor::Visit(PerspectiveCamera * cameraToVisit)
	{
		// Void
	}

	void UpdateVisitor::RegisterTime()
	{
		double newTime = glfwGetTime();
		_timeSinceLastUpdate = newTime - _timeAtLastUpdate;
		_timeAtLastUpdate = newTime;
	}
}