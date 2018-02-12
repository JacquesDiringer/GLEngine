#include <stdafx.h>
#include <Render\RenderableCollectorVisitor.h>


namespace GLEngine
{
	RenderableCollectorVisitor::RenderableCollectorVisitor()
	{
		_collectedElements = new RenderingElements();
	}

	RenderableCollectorVisitor::~RenderableCollectorVisitor()
	{
	}

	void RenderableCollectorVisitor::Visit(Model * modelToVisit)
	{
		_collectedElements->PushModel(modelToVisit);
	}

	void RenderableCollectorVisitor::Visit(EnvironmentMapSky * skyToVisit)
	{
		// TODO: Make this nullptr work !
		/*if (_collectedElements->GetSky() == nullptr)
		{
			_collectedElements->SetSky(skyToVisit);
		}
		else
		{
			throw new std::exception("There is more than one EnvironmentMapSky in the scene tree.");
		}*/

		_collectedElements->SetSky(skyToVisit);
	}

	void RenderableCollectorVisitor::Visit(PointLight * pointLightToVisit)
	{
		_collectedElements->PushPointLight(pointLightToVisit);
	}

	void RenderableCollectorVisitor::Visit(Actor * actorToVisit)
	{
		// Void
	}

	void RenderableCollectorVisitor::Visit(PerspectiveCamera * cameraToVisit)
	{
		// Void
	}

	bool RenderableCollectorVisitor::CheckAndResetCollection()
	{
		// TODO: this is temporary.
		_collectedElements->Clear();

		_collectedElements->SetSky(nullptr);

		return true;
	}
}