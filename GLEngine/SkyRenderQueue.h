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

		virtual void SetGpuState() const; // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void ResetGpuDefaultState() const; // Post rendering. Resets the GPU to it's original state.

	private:
		mutable GLboolean _oldDepthTestEnabled;
		mutable GLboolean _oldCullingEnabled;
		mutable GLint _oldCullFace;
	};
}

