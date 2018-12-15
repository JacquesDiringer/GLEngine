#pragma once

namespace GLEngine
{
	class SceneNode;
	class Actor;
	class Model;
	class EnvironmentMapSky;
	class PointLight;
	class PerspectiveCamera;
	class CellArray;
	class Cell;

	class SceneElementVisitor
	{
	public:
		SceneElementVisitor();
		~SceneElementVisitor();

		// Visitor pattern.
		// The SceneNode overload is common to a lot of visitor so it needs to be overriden by those who need to do something more specific.
		virtual void Visit(SceneNode* sceneNodeToVisit);
		virtual void Visit(Model* modelToVisit) = 0;
		virtual void Visit(EnvironmentMapSky* skyToVisit) = 0;
		virtual void Visit(PointLight* pointLightToVisit) = 0;
		virtual void Visit(Actor* actorToVisit) = 0;
		virtual void Visit(PerspectiveCamera* cameraToVisit) = 0;
		virtual void Visit(CellArray* cellArrayToVisit);
		virtual void Visit(Cell* cellToVisit);
	};
}