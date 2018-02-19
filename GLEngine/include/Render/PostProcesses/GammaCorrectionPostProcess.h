#pragma once
#include <Render\PostProcesses\PostProcess.h>
namespace GLEngine
{
	class GammaCorrectionPostProcess :
		public PostProcess
	{
	public:
		GammaCorrectionPostProcess(int width, int height, TextureManager* textureManager);
		~GammaCorrectionPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

	};

}