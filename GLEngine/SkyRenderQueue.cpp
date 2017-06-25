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

	void SkyRenderQueue::SetGpuState()
	{
		// Remember the old states.
		glGetBooleanv(GL_DEPTH_TEST, &_oldDepthTestEnabled);
		glGetBooleanv(GL_CULL_FACE, &_oldCullingEnabled);
		glGetIntegerv(GL_CULL_FACE_MODE, &_oldCullFace);

		// Disable depth testing.
		if (_oldDepthTestEnabled)
		{
			glDisable(GL_DEPTH_TEST);
		}

		// Face culling.
		if (!_oldCullingEnabled)
		{
			glEnable(GL_CULL_FACE);
		}

		if (_oldCullFace != GL_FRONT)
		{
			glCullFace(GL_FRONT);
		}
	}

	void SkyRenderQueue::ResetGpuDefaultState()
	{
		if (_oldDepthTestEnabled)
		{
			glEnable(GL_DEPTH_TEST);
		}

		if (!_oldCullingEnabled)
		{
			glEnable(_oldCullingEnabled);
		}

		if (_oldCullFace != GL_FRONT)
		{
			glCullFace(_oldCullFace);
		}
	}

}