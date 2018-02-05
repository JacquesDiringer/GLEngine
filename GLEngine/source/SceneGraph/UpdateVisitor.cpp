#include <stdafx.h>
#include <SceneGraph\UpdateVisitor.h>
#include <SceneGraph\SceneElement.h>
#include <SceneGraph\SceneNode.h>
#include <Actor\Actor.h>
#include <Render\Model.h>
#include <Render\EnvironmentMapSky.h>

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