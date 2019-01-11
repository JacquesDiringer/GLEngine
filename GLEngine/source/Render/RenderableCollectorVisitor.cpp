#include <stdafx.h>
#include <Render\RenderableCollectorVisitor.h>
#include <SceneGraph\Cell.h>
#include <math.h>
#include <iostream>

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
		//if (NodeIsInFrustum(*sceneNodeToVisit))
		if (true)
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

	void RenderableCollectorVisitor::Visit(Cell * cellToVisit)
	{
		// Frustum culling happens here.
		// It is a bit different for cells than scene nodes.
		if (CellIsInFrustum(*cellToVisit))
		{
			//std::cout << "Cell in frustum" << std::endl;
			SceneElementVisitor::Visit(cellToVisit);
			_totalCellsCount++;
		}
		else
		{
			//std::cout << "Cell not in frustum" << std::endl;
			_totalCellsNotRendered++;
		}
	}

	bool RenderableCollectorVisitor::CheckAndResetCollection()
	{
		// TODO: this is temporary.
		_collectedElements->Clear();

		// Reset statistics data.
		_totalVertexCount = 0;
		_totalPrimitiveCount = 0;
		_totalObjectCount = 0;
		_totalCellsCount = 0;
		_totalCellsNotRendered = 0;

		// Switch from an odd frame to an even one, then back.
		IncrementCurrentFrameId();

		return true;
	}

	bool RenderableCollectorVisitor::NodeIsInFrustum(SceneNode& node) const
	{
		float boundingRadius = node.GetBoundingSphereRadius();

		GLEngineMath::Vector3 boundingRadiusVector = GLEngineMath::Vector3(boundingRadius, boundingRadius, -boundingRadius);

		GLEngineMath::Matrix4::Multiply(_camera->GetView(), node.GetWorldTransformation().Position(), _projectedPointMinViewSpace);
		_projectedPointMinViewSpace -= boundingRadiusVector;
		GLEngineMath::Matrix4::Multiply(_camera->GetView(), node.GetWorldTransformation().Position(), _projectedPointMaxViewSpace);
		_projectedPointMaxViewSpace += boundingRadiusVector;

		GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), _projectedPointMinViewSpace, _projectedPointMin);
		GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), _projectedPointMaxViewSpace, _projectedPointMax);

		return
			// Test first if the points are not behind the camera.
			// When a coordinate in projection space,= is tested, we have to first make sure that it doesn't lie behind the camera, otherwise the projected coordinates are degenerated.

			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.Z() < 1) && // Far plane
			(_projectedPointMaxViewSpace.Z() < 0 && _projectedPointMax.Z() > -1) && // Near plane
			(_projectedPointMaxViewSpace.Z() > 0 || _projectedPointMax.X() > -1) && // Left plane
			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.X() < 1) && // Right plane
			(_projectedPointMaxViewSpace.Z() > 0 || _projectedPointMax.Y() > -1) && // Bottom plane
			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.Y() < 1); // Top plane
	}

	bool RenderableCollectorVisitor::CellIsInFrustum(const Cell & cell) const
	{
		// Radius of the sphere containing the cell.
		float sphereRadius = cell.GetSize() * sqrtf(3.0f);

		GLEngineMath::Vector3 sphereRadiusVector = GLEngineMath::Vector3(sphereRadius, sphereRadius, -sphereRadius);

		GLEngineMath::Matrix4::Multiply(_camera->GetView(), cell.GetPosition(), _projectedPointMinViewSpace);
		_projectedPointMinViewSpace -= sphereRadiusVector;
		GLEngineMath::Matrix4::Multiply(_camera->GetView(), cell.GetPosition(), _projectedPointMaxViewSpace);
		_projectedPointMaxViewSpace += sphereRadiusVector;

		GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), _projectedPointMinViewSpace, _projectedPointMin);
		GLEngineMath::Matrix4::Multiply(_camera->GetProjection(), _projectedPointMaxViewSpace, _projectedPointMax);

		return
			// Test first if the points are not behind the camera.
			// When a coordinate in projection space,= is tested, we have to first make sure that it doesn't lie behind the camera, otherwise the projected coordinates are degenerated.

			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.Z() < 1) && // Far plane
			(_projectedPointMaxViewSpace.Z() < 0 && _projectedPointMax.Z() > -1) && // Near plane
			(_projectedPointMaxViewSpace.Z() > 0 || _projectedPointMax.X() > -1) && // Left plane
			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.X() < 1) && // Right plane
			(_projectedPointMaxViewSpace.Z() > 0 || _projectedPointMax.Y() > -1) && // Bottom plane
			(_projectedPointMinViewSpace.Z() > 0 || _projectedPointMin.Y() < 1); // Top plane
	}
}