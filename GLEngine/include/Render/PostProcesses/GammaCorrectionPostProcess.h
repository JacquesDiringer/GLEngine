#pragma once
#include <Render\PostProcesses\PostProcess.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API GammaCorrectionPostProcess :
		public PostProcess
	{
	public:
		GammaCorrectionPostProcess(int width, int height, TextureManager* textureManager);
		~GammaCorrectionPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

	};

}