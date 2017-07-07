#pragma once

#include "SceneManager.h"
#include "RenderableCollectorVisitor.h"
#include "NoAlphaRenderQueue.h"
#include "SkyRenderQueue.h"

namespace GLEngine
{
	class RenderManager
	{
	public:
		RenderManager(int width, int height);
		~RenderManager();

		void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

		void SetIblTexture(Texture2D* iblTexture) { _iblTexture = iblTexture; }

	private:
		int _viewportWidth, _viewportHeight;
		RenderableCollectorVisitor* _collectorVisitor;
		NoAlphaRenderQueue* _modelsRenderQueue;
		SkyRenderQueue* _skyRenderQueue;
		int _frameCount = 0;

		// G-Buffer frame buffer id.
		GLuint _gBuffer;
		// G-Buffer textures ids.
		GLuint _geometryTexture, _diffuseTexture, _specularRoughnessTexture, _emissiveTexture;

		// Lighting frame buffer id.
		GLuint _lightingBuffer;
		// Lighting texture id.
		GLuint _lightingTexture;

		Texture2D* _iblTexture;

		// Intializes buffers for deferred rendering.
		void InitializeFrameBuffers();
	};
}