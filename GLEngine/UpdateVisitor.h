#pragma once
#include "SceneElementVisitor.h"

namespace GLEngine
{
	class UpdateVisitor :
		public SceneElementVisitor
	{
	public:
		UpdateVisitor();
		~UpdateVisitor();

		// Visitor pattern.
		virtual void Visit(Model* modelToVisit);
		virtual void Visit(EnvironmentMapSky* skyToVisit);
		virtual void Visit(Actor* actorToVisit);

		void RegisterTime();
		float GetTimeSinceLastUpdate()const { return (float)_timeSinceLastUpdate; }

	private:
		double _timeSinceLastUpdate;
		double _timeAtLastUpdate;
	};
}

