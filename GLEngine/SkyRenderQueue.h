#pragma once
#include "RenderQueue.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLEngine
{
	class SkyRenderQueue :
		public RenderQueue
	{
	public:
		SkyRenderQueue();
		~SkyRenderQueue();

		virtual void SetGpuState(); // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void ResetGpuDefaultState(); // Post rendering. Resets the GPU to it's original state.

	private:
		GLboolean _oldDepthTestEnabled;
		GLboolean _oldCullingEnabled;
		GLint _oldCullFace;
	};
}

