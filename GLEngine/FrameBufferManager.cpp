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
		glBindFramebuffer(GL_FRAMEBUFFER, bufferToBind->GetId());
		_boundFrameBuffer = bufferToBind;
	}

	void FrameBufferManager::SetDefaultFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_boundFrameBuffer = nullptr;
	}

}