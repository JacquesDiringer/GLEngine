#pragma once
#include "RenderQueue.h"

namespace GLEngine
{
	class PointLightRenderQueue :
		public RenderQueue
	{
	public:
		PointLightRenderQueue();
		~PointLightRenderQueue();

		virtual void SetGpuState(); // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void ResetGpuDefaultState(); // Post rendering. Resets the GPU to it's original state.

	private:
		GLboolean _oldDepthTestEnabled;
		GLint _oldDepthFunc;
		GLboolean _oldDepthMask;
		GLboolean _oldCullingEnabled;
		GLint _oldCullFace;
	};

}
