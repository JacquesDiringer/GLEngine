#pragma once
#include <Render\PostProcesses\PostProcess.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API LensPostProcess :
		public PostProcess
	{
	public:
		LensPostProcess(int width, int height, TextureManager* textureManager);
		~LensPostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

	private:
		Texture2D* _downscaledGhostTexture = nullptr;
		Texture2D* _bluxXTexture = nullptr;
		Texture2D* _bluxYTexture = nullptr;

		GLEngineMath::Vector2 _pixelSize;
		float _mipLevelForGhosts;
		int _blurSamplesCount;
	};
}