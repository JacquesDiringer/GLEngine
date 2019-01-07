#pragma once
#include <Render\PostProcesses\PostProcess.h>
#include <Render\RenderManager.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API FogPostProcess :
		public PostProcess
	{
	public:
		FogPostProcess(int width, int height, TextureManager* textureManager, const RenderManager& renderManager);
		~FogPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;


	private:
		Texture2D * _gBufferGeometryTexture;
	};

}