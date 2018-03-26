#pragma once
#include <Render\PostProcesses\PostProcess.h>
namespace GLEngine
{
	class LensPostProcess :
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