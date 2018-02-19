#pragma once
#include <Render\PostProcesses\PostProcess.h>
namespace GLEngine
{
	class BloomPostProcess :
		public PostProcess
	{
	public:
		BloomPostProcess(int width, int height, TextureManager* textureManager);
		~BloomPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;
	};
}