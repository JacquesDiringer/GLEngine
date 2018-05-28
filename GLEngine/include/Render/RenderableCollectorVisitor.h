#pragma once

#include <SceneGraph\SceneElementVisitor.h>
#include <Render\RenderingElements.h>

namespace GLEngine
{
	class RenderableCollectorVisitor
		: public SceneElementVisitor
	{
	public:
		RenderableCollectorVisitor();
		~RenderableCollectorVisitor();

		RenderingElements* GetCollectedElements() const { return _collectedElements; }

		int GetTotalPrimitiveCount() const { return _totalPrimitiveCount; }
		int GetTotalVertexCount() const { return _totalVertexCount; }
		int GetTotalObjectCount() const { return _totalObjectCount; }

		void SetCamera(PerspectiveCamera * camera) { _camera = camera; }

		// Visitor pattern.
		virtual void Visit(SceneNode* sceneNodeToVisit);
		virtual void Visit(Model* modelToVisit);
		virtual void Visit(EnvironmentMapSky* skyToVisit);
		virtual void Visit(PointLight* pointLightToVisit);
		virtual void Visit(Actor* actorToVisit);
		virtual void Visit(PerspectiveCamera* cameraToVisit);

		// Returns true if collections were empty and things were reset.
		bool CheckAndResetCollection();

	private:
		bool PointIsInFrustum(SceneNode& node) const;

	private:
		PerspectiveCamera * _camera;
		RenderingElements* _collectedElements = nullptr;
		int _totalPrimitiveCount, _totalVertexCount, _totalObjectCount;
	};
}