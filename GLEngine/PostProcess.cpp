#include "stdafx.h"
#include "PostProcess.h"


namespace GLEngine
{
	PostProcess::PostProcess(int width, int height, FrameBufferManager* frameBufferManager)
	{
		_processedFrameBuffer = new RGB16FBuffer(width, height, frameBufferManager);
	}


	PostProcess::~PostProcess()
	{
	}
}