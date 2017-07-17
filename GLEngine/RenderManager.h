#pragma once

#include "SceneManager.h"
#include "RenderableCollectorVisitor.h"
#include "NoAlphaRenderQueue.h"
#include "SkyRenderQueue.h"
#include "EnvironmentMapLight.h"

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

		// G-Buffer frame buffer id.
		GLuint _gBuffer;
		// G-Buffer textures ids.
		Texture2D *_geometryTexture, *_diffuseTexture, *_specularRoughnessTexture, *_emissiveTexture;

		// Lighting frame buffer id.
		GLuint _lightingBuffer;
		// Lighting texture.
		Texture2D * _lightingTexture;

		// Environment map light Renderable that will be consistent from one frame to an other, should only need to be calculated once.
		EnvironmentMapLight* _environmentMapLight = nullptr;

		// Intializes buffers for deferred rendering.
		void InitializeFrameBuffers();
	};
}