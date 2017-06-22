#pragma once
#include "SceneElementVisitor.h"
#include"SceneManager.h"

namespace GLEngine
{
	class RenderVisitor :
		public SceneElementVisitor
	{
	public:
		RenderVisitor();
		RenderVisitor(SceneManager* sceneManager);
		~RenderVisitor();

		// Visitor pattern.
		virtual void Visit(Model* modelToVisit);
		virtual void Visit(EnvironmentMapSky* skyToVisit);
		virtual void Visit(Actor* actorToVisit);

		void SetSceneManager(SceneManager* sceneManager) { _sceneManager = sceneManager; }

	private:
		SceneManager* _sceneManager;
	};
}