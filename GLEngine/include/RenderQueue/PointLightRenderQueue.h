#pragma once
#include <RenderQueue\RenderQueue.h>

namespace GLEngine
{
	class PointLightRenderQueue :
		public RenderQueue
	{
	public:
		PointLightRenderQueue();
		~PointLightRenderQueue();

		virtual void SetGpuState(GraphicsResourceManager* graphicsResourceManager) const; // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void ResetGpuDefaultState(GraphicsResourceManager* graphicsResourceManager) const; // Post rendering. Resets the GPU to it's original state.

	private:
		mutable GLboolean _oldDepthTestEnabled;
		mutable GLint _oldDepthFunc;
		mutable GLboolean _oldDepthMask;
		mutable GLboolean _oldCullingEnabled;
		mutable GLint _oldCullFace;
	};

}
