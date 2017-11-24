#include "stdafx.h"
#include "PointLightRenderQueue.h"


namespace GLEngine
{
	PointLightRenderQueue::PointLightRenderQueue()
		: RenderQueue()
	{
	}


	PointLightRenderQueue::~PointLightRenderQueue()
	{
	}

	void PointLightRenderQueue::SetGpuState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		// Remember the old states.
		_oldDepthTestEnabled = graphicsDeviceManager->GetBooleanState(GL_DEPTH_TEST);
		_oldDepthFunc = graphicsDeviceManager->GetDepthFunc();
		_oldDepthMask = graphicsDeviceManager->GetDepthWriteMask();
		_oldCullingEnabled = graphicsDeviceManager->GetBooleanState(GL_CULL_FACE);
		_oldCullFace = graphicsDeviceManager->GetCullFace();

		// Enable depth testing and set it to be reversed (only draw if the object is behind a surface).
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, true);
		graphicsDeviceManager->SetDepthFunc(GL_GREATER);

		// Disable depth writing.
		graphicsDeviceManager->SetDepthWriteMask(false);

		// Face culling.
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, true);

		// Front culling: we want to see the interior of the sphere only.
		graphicsDeviceManager->SetCullFace(GL_FRONT);
	}

	void PointLightRenderQueue::ResetGpuDefaultState(GraphicsResourceManager* graphicsResourceManager) const
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		// Reverse to the old states.
		graphicsDeviceManager->SetBooleanState(GL_DEPTH_TEST, _oldDepthTestEnabled);
		graphicsDeviceManager->SetDepthFunc(_oldDepthFunc);
		graphicsDeviceManager->SetDepthWriteMask(_oldDepthMask);
		graphicsDeviceManager->SetBooleanState(GL_CULL_FACE, _oldCullingEnabled);
		graphicsDeviceManager->SetCullFace(_oldCullFace);
	}

}