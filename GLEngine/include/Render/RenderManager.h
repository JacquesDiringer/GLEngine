#pragma once

#include <SceneGraph\SceneManager.h>
#include <Render\RenderableCollectorVisitor.h>
#include <RenderQueue\NoAlphaRenderQueue.h>
#include <RenderQueue\PointLightRenderQueue.h>
#include <RenderQueue\SkyRenderQueue.h>
#include <Render\Lighting\EnvironmentMapLight.h>
#include <FrameBuffer\GBuffer.h>
#include <FrameBuffer\RGB16FBuffer.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API RenderManager
	{
	public:
		RenderManager(int width, int height, GraphicsResourceManager* graphicsResourceManager);
		~RenderManager();

		void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

		Texture2D* GetGBufferGeometryTexture() const { return _gBuffer->GetGeomtryTexture(); }

	private:
		int _viewportWidth, _viewportHeight;
		RenderableCollectorVisitor* _collectorVisitor;
		NoAlphaRenderQueue* _modelsRenderQueue;
		PointLightRenderQueue* _pointLightsRenderQueue;
		SkyRenderQueue* _skyRenderQueue;

		// G-Buffer frame buffer.
		GBuffer* _gBuffer;

		// Lighting frame buffer.
		RGB16FBuffer* _lightingBuffer;

		// Combine frame buffer. Render obtained before post processing.
		RGB16FBuffer* _combineBuffer;

		// Post processes ping pong buffers.
		// We need 2 buffers to swap between them when there are more than one post process.
		RGB16FBuffer * _postProcessEvenBuffer, * _postProcessOddBuffer;

		// Environment map light Renderable that will be consistent from one frame to an other, should only need to be calculated once.
		mutable EnvironmentMapLight* _environmentMapLight = nullptr;

		// Intializes buffers for deferred rendering.
		void InitializeFrameBuffers(GraphicsResourceManager* graphicsResourceManager);

		mutable int _frameCount = 0;
	};
}