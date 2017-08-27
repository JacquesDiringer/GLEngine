#include "stdafx.h"
#include "PostProcess.h"


namespace GLEngine
{
	PostProcess::PostProcess(int width, int height)
	{
		_processedFrameBuffer = new RGB16FBuffer(width, height);
	}


	PostProcess::~PostProcess()
	{
	}
}