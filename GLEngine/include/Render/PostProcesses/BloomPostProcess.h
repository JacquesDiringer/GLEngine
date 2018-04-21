#pragma once
#include <Render\PostProcesses\PostProcess.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API BloomPostProcess :
		public PostProcess
	{
	public:
		BloomPostProcess(int width, int height, TextureManager* textureManager);
		~BloomPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;
	};
}