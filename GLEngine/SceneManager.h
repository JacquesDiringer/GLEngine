#pragma once

#include"SceneNode.h"
#include "PerspectiveCamera.h"
#include "RenderingElements.h"
#include "UpdateVisitor.h"

namespace GLEngine
{
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

		// Browse the scene graph to collect all elements that will contribute to the rendering.
		RenderingElements* CollectElementsForRendering();

	private:
		SceneNode* _rootNode;
		PerspectiveCamera* _currentCamera;
		UpdateVisitor* _updateVisitor;
	};
}

