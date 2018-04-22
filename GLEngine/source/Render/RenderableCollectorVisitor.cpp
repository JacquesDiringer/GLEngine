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

		// Add the amount of vertices, primitives and objects to the total, for statistic purposes.
		_totalVertexCount += modelToVisit->GetResource()->GetVao()->GetVertexCount();
		_totalPrimitiveCount += modelToVisit->GetResource()->GetVao()->GetElementsCount();
		_totalObjectCount++;
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

		// Reset statistics data.
		_totalVertexCount = 0;
		_totalPrimitiveCount = 0;
		_totalObjectCount = 0;

		return true;
	}
}