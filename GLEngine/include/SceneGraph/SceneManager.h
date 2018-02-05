#pragma once

#include <SceneGraph\SceneNode.h>
#include <Camera\PerspectiveCamera.h>
#include <SceneGraph\UpdateVisitor.h>

namespace GLEngine
{
	class RenderingElements;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		SceneNode* GetRootNode() const { return _rootNode; }

		void SetCurrentCamera(PerspectiveCamera* camera) { _currentCamera = camera; }
		PerspectiveCamera* GetCurrentCamera() { return _currentCamera; }

		// Updates the scene graph. Elements should be up to date before being collected.
		void Update();

	private:
		SceneNode* _rootNode;
		PerspectiveCamera* _currentCamera;
		UpdateVisitor* _updateVisitor;
	};
}

