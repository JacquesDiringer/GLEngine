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

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

	private:
		RGB16FBuffer* _downscaledGhostBuffer = nullptr;
		RGB16FBuffer* _bluxXBuffer = nullptr;
		RGB16FBuffer* _bluxYBuffer = nullptr;

		Vector2* _pixelSize = nullptr;
		float _mipLevelForGhosts;
		int _blurSamplesCount;
	};
}