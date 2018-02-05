#include <stdafx.h>
#include <RenderQueue\NoAlphaRenderQueue.h>


namespace GLEngine
{
	NoAlphaRenderQueue::NoAlphaRenderQueue()
		: RenderQueue()
	{
	}

	NoAlphaRenderQueue::~NoAlphaRenderQueue()
	{
	}

	void NoAlphaRenderQueue::SetGpuState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		//// Remember the old states.
		_oldDepthTestEnabled = graphicsDeviceManager->GetBooleanState(GL_DEPTH_TEST);
		_oldDepthMask = graphicsDeviceManager->GetDepthWriteMask();
		_oldCullingEnabled = graphicsDeviceManager->GetBooleanState(GL_CULL_FACE);
		_oldCullFace = graphicsDeviceManager->GetCullFace();


		//// Enable depth testing and set it.
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, true);

		//// Enable depth writing.
		graphicsDeviceManager->SetDepthWriteMask(true);

		//// Face culling.
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, true);
		graphicsDeviceManager->SetCullFace(GL_BACK);
	}

	void NoAlphaRenderQueue::ClearRenderables()
	{
		// Delete all objects in the list.
		for each (Renderable* currentRenderable in _renderables)
		{
			currentRenderable->~Renderable();
		}

		_renderables.clear();
	}

	void NoAlphaRenderQueue::ResetGpuDefaultState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		//// Go back to the old states.
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, _oldDepthTestEnabled);
		graphicsDeviceManager->SetDepthWriteMask(_oldDepthMask);
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, _oldCullingEnabled);
		graphicsDeviceManager->SetCullFace(_oldCullFace);
	}
}