#include <stdafx.h>
#include <Render\PostProcesses\PostProcess.h>
#include <Texture\Texture2DRGBA16F.h>

namespace GLEngine
{
	PostProcess::PostProcess(int width, int height, TextureManager* textureManager)
		: _textureManager(textureManager)
	{
		_outputTexture = new Texture2DRGBA16F(width, height, (void*)NULL);
	}

	PostProcess::~PostProcess()
	{
	}
}