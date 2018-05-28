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

	void RenderableCollectorVisitor::Visit(SceneNode * sceneNodeToVisit)
	{
		// Frustum culling happens here.
		if (PointIsInFrustum(*sceneNodeToVisit))
		{
			SceneElementVisitor::Visit(sceneNodeToVisit);
		}
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
		// Void
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

		// Reset statistics data.
		_totalVertexCount = 0;
		_totalPrimitiveCount = 0;
		_totalObjectCount = 0;

		return true;
	}

	bool RenderableCollectorVisitor::PointIsInFrustum(SceneNode& node) const
	{
		float boundingRadius = node.GetBoundingSphereRadius();

		GLEngineMath::Vector3& projectedPointMinViewSpace = GLEngineMath::Matrix4::Multiply(_camera->GetView(), node.GetWorldTransformation().Position()) - GLEngineMath::Vector3(boundingRadius, boundingRadius, -boundingRadius);
		GLEngineMath::Vector3& projectedPointMaxViewSpace = GLEngineMath::Matrix4::Multiply(_camera->GetView(), node.GetWorldTransformation().Position()) + GLEngineMath::Vector3(boundingRadius, boundingRadius, -boundingRadius);

		GLEngineMath::Vector3& projectedPointMin = GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), projectedPointMinViewSpace);
		GLEngineMath::Vector3& projectedPointMax = GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), projectedPointMaxViewSpace);

		return
			// Test first if the points are not behind the camera.
			// When a coordinate in projection space,= is tested, we have to first make sure that it doesn't lie behind the camera, otherwise the projected coordinates are degenerated.

			(projectedPointMinViewSpace.Z() > 0 || projectedPointMin.Z() < 1) && // Far plane
			(projectedPointMaxViewSpace.Z() < 0 && projectedPointMax.Z() > -1) && // Near plane
			(projectedPointMaxViewSpace.Z() > 0 || projectedPointMax.X() > -1) && // Left plane
			(projectedPointMinViewSpace.Z() > 0 || projectedPointMin.X() < 1) && // Right plane
			(projectedPointMaxViewSpace.Z() > 0 || projectedPointMax.Y() > -1) && // Bottom plane
			(projectedPointMinViewSpace.Z() > 0 || projectedPointMin.Y() < 1); // Top plane
	}
}