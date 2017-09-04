#pragma once
#include "PostProcess.h"
namespace GLEngine
{
	class GammaCorrectionPostProcess :
		public PostProcess
	{
	public:
		GammaCorrectionPostProcess(int width, int height);
		~GammaCorrectionPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	};

}