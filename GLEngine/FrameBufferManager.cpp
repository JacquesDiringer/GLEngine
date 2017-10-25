#include "stdafx.h"
#include "FrameBufferManager.h"

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

namespace GLEngine
{
	FrameBufferManager::FrameBufferManager()
	{
	}


	FrameBufferManager::~FrameBufferManager()
	{
	}

	void FrameBufferManager::Bind(FrameBuffer * bufferToBind)
	{
		if (bufferToBind != nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, bufferToBind->GetId());
			_boundFrameBuffer = bufferToBind;
			_defaultBound = false;
		}
		else
		{
			SetDefaultFrameBuffer();
		}
	}

	void FrameBufferManager::SetDefaultFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_boundFrameBuffer = nullptr;
		_defaultBound = true;
	}

}