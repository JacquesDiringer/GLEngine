#pragma once
#include "PostProcess.h"
namespace GLEngine
{
	class LensPostProcess :
		public PostProcess
	{
	public:
		LensPostProcess(int width, int height, FrameBufferManager* frameBufferManager);
		~LensPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		RGB16FBuffer* _downscaledGhostBuffer = nullptr;
	};
}