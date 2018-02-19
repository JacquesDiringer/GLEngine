#include <stdafx.h>
#include <Render\PostProcesses\PostProcess.h>


namespace GLEngine
{
	PostProcess::PostProcess(int width, int height, TextureManager* textureManager)
		: _textureManager(textureManager)
	{
		_outputTexture = new Texture2DRGB16F(width, height, (void*)NULL);
	}

	PostProcess::~PostProcess()
	{
	}
}