#include "stdafx.h"
#include "SkyRenderQueue.h"


namespace GLEngine
{
	SkyRenderQueue::SkyRenderQueue()
		: RenderQueue()
	{
	}


	SkyRenderQueue::~SkyRenderQueue()
	{
	}

	void SkyRenderQueue::SetGpuState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		// Remember the old states.
		_oldDepthTestEnabled = graphicsDeviceManager->GetBooleanState(GL_DEPTH_TEST);
		_oldCullingEnabled = graphicsDeviceManager->GetBooleanState(GL_CULL_FACE);
		_oldCullFace = graphicsDeviceManager->GetCullFace();

		// Disable depth testing.
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, false);

		// Face culling.
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, true);

		// We want to see the interior of the sky's sphere.
		graphicsDeviceManager->SetCullFace(GL_FRONT);
	}

	void SkyRenderQueue::ResetGpuDefaultState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		// Reverse to the old states.
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, _oldDepthTestEnabled);
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, _oldCullingEnabled);
		graphicsDeviceManager->SetCullFace(_oldCullFace);
	}

}