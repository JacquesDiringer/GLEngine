#pragma once
#include "PostProcess.h"
namespace GLEngine
{
	class LensPostProcess :
		public PostProcess
	{
	public:
		LensPostProcess(int width, int height);
		~LensPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);
	};
}