#pragma once
#include "PostProcess.h"
namespace GLEngine
{
	class BloomPostProcess :
		public PostProcess
	{
	public:
		BloomPostProcess(int width, int height, FrameBufferManager* frameBufferManager);
		~BloomPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);
	};
}