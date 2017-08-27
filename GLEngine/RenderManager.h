#pragma once

#include "SceneManager.h"
#include "RenderableCollectorVisitor.h"
#include "NoAlphaRenderQueue.h"
#include "SkyRenderQueue.h"
#include "EnvironmentMapLight.h"
#include "GBuffer.h"
#include "RGB16FBuffer.h"

namespace GLEngine
{
	class RenderManager
	{
	public:
		RenderManager(int width, int height);
		~RenderManager();

		void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		int _viewportWidth, _viewportHeight;
		RenderableCollectorVisitor* _collectorVisitor;
		NoAlphaRenderQueue* _modelsRenderQueue;
		SkyRenderQueue* _skyRenderQueue;

		// G-Buffer frame buffer.
		GBuffer* _gBuffer;

		// Lighting frame buffer.
		RGB16FBuffer* _lightingBuffer;

		// Combine frame buffer. Render obtained before post processing.
		RGB16FBuffer* _combineBuffer;

		// Environment map light Renderable that will be consistent from one frame to an other, should only need to be calculated once.
		EnvironmentMapLight* _environmentMapLight = nullptr;

		// Intializes buffers for deferred rendering.
		void InitializeFrameBuffers();
	};
}