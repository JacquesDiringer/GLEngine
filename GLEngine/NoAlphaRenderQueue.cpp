#include "stdafx.h"
#include "NoAlphaRenderQueue.h"


namespace GLEngine
{
	NoAlphaRenderQueue::NoAlphaRenderQueue()
		: RenderQueue()
	{
	}

	NoAlphaRenderQueue::~NoAlphaRenderQueue()
	{
	}

	void NoAlphaRenderQueue::SetGpuState()
	{
		// Remember the old states.
		glGetBooleanv(GL_DEPTH_TEST, &_oldDepthTestEnabled);
		glGetBooleanv(GL_DEPTH_WRITEMASK, &_oldDepthMask);
		glGetBooleanv(GL_CULL_FACE, &_oldCullingEnabled);
		glGetIntegerv(GL_CULL_FACE_MODE, &_oldCullFace);

		// Enable depth testing and set it.
		// Only if the state actually needs to be changed.
		if (!_oldDepthTestEnabled)
		{
			glEnable(GL_DEPTH_TEST);
		}

		// Enable depth writing.
		if (!_oldDepthMask)
		{
			glDepthMask(GL_TRUE);
		}

		// Face culling.
		if (!_oldCullingEnabled)
		{
			glEnable(GL_CULL_FACE);
		}

		if (_oldCullFace != GL_BACK)
		{
			glCullFace(GL_BACK);
		}
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

	void NoAlphaRenderQueue::ResetGpuDefaultState()
	{
		if (!_oldDepthTestEnabled)
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (!_oldDepthMask)
		{
			glDepthMask(_oldDepthMask);
		}

		if (!_oldCullingEnabled)
		{
			glEnable(_oldCullingEnabled);
		}

		if (_oldCullFace != GL_BACK)
		{
			glCullFace(_oldCullFace);
		}
	}
}