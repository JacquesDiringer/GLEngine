#pragma once

#include"SceneNode.h"
#include "PerspectiveCamera.h"
#include "UpdateVisitor.h"

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
		void SetLeftCamera(PerspectiveCamera* camera) { _leftCamera = camera; }
		void SetRightCamera(PerspectiveCamera* camera) { _rightCamera = camera; }
		PerspectiveCamera* GetCurrentCamera() { return _currentCamera; }
		PerspectiveCamera* GetLeftCamera() { return _leftCamera; }
		PerspectiveCamera* GetRightCamera() { return _rightCamera; }

		void SetLeftEyeRendered(const bool value) { _leftEyeRendered = value; }
		bool GetLeftEyeRendered() const { return _leftEyeRendered; }

		// Updates the scene graph. Elements should be up to date before being collected.
		void Update();

	private:
		SceneNode* _rootNode;
		PerspectiveCamera* _currentCamera, *_leftCamera, *_rightCamera;
		UpdateVisitor* _updateVisitor;
		bool _leftEyeRendered = true;
	};
}

