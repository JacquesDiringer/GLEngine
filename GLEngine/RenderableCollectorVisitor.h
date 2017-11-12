#pragma once

#include "SceneElementVisitor.h"
#include "RenderingElements.h"

namespace GLEngine
{
	class RenderableCollectorVisitor
		: public SceneElementVisitor
	{
	public:
		RenderableCollectorVisitor();
		~RenderableCollectorVisitor();

		RenderingElements* GetCollectedElements() const { return _collectedElements; }

		// Visitor pattern.
		virtual void Visit(Model* modelToVisit);
		virtual void Visit(EnvironmentMapSky* skyToVisit);
		virtual void Visit(PointLight* pointLightToVisit);
		virtual void Visit(Actor* actorToVisit);
		virtual void Visit(PerspectiveCamera* cameraToVisit);

		// Returns true if collections were empty and things were reset.
		bool CheckAndResetCollection();

	private:
		RenderingElements* _collectedElements = nullptr;
	};
}