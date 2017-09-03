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

	void PointLightRenderQueue::SetGpuState()
	{
		// Remember the old states.
		glGetBooleanv(GL_DEPTH_TEST, &_oldDepthTestEnabled);
		glGetIntegerv(GL_DEPTH_FUNC, &_oldDepthFunc);
		glGetBooleanv(GL_DEPTH_WRITEMASK, &_oldDepthMask);
		glGetBooleanv(GL_CULL_FACE, &_oldCullingEnabled);
		glGetIntegerv(GL_CULL_FACE_MODE, &_oldCullFace);

		// Enable depth testing and set it to be reversed (only draw if the object is behind a surface).
		// Only if the state actually needs to be changed.
		if (!_oldDepthTestEnabled)
		{
			glEnable(GL_DEPTH_TEST);
		}

		if (_oldDepthFunc != GL_GREATER)
		{
			glDepthFunc(GL_GREATER);
		}

		// Disable depth writing.
		if (_oldDepthMask)
		{
			glDepthMask(GL_FALSE);
		}

		// Face culling.
		if (!_oldCullingEnabled)
		{
			glEnable(GL_CULL_FACE);
		}

		// Front culling: we want to see the interior of the sphere only.
		if (_oldCullFace != GL_FRONT)
		{
			glCullFace(GL_FRONT);
		}
	}

	void PointLightRenderQueue::ResetGpuDefaultState()
	{
		if (!_oldDepthTestEnabled)
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (_oldDepthFunc != GL_GREATER)
		{
			glDepthFunc(_oldDepthFunc);
		}

		if (_oldDepthMask)
		{
			glDepthMask(_oldDepthMask);
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