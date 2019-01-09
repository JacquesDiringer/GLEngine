#pragma once

#include <SceneGraph\SceneNode.h>
#include <Camera\PerspectiveCamera.h>
#include <SceneGraph\UpdateVisitor.h>

#include <unordered_set>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class RenderingElements;

	class GLENGINE_API SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		SceneNode* GetRootNode() const { return _rootNode; }

		EnvironmentMapSky* GetSky() const { return _sky; }
		void SetSky(EnvironmentMapSky* sky) { _sky = sky; }

		void SetCurrentCamera(PerspectiveCamera* camera) { _currentCamera = camera; }
		PerspectiveCamera* GetCurrentCamera() { return _currentCamera; }

		// Updates the scene graph. Elements should be up to date before being collected.
		void Update();

		void AddActor(Actor * actorToAdd) { _actorElements.insert(actorToAdd); }
		void RemoveActor(Actor * actorToRemove) { _actorElements.erase(actorToRemove); }

	private:
		SceneNode* _rootNode;
		EnvironmentMapSky* _sky;
		PerspectiveCamera* _currentCamera;
		UpdateVisitor* _updateVisitor;
		std::unordered_set<Actor*> _actorElements;
	};
}

