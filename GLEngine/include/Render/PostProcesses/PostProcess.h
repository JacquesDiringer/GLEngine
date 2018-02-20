#pragma once
#include <Render\Renderable.h>
#include <FrameBuffer\FrameBufferManager.h>
#include <Texture\Texture2D.h>

namespace GLEngine
{
	class PostProcess
		: public Renderable
	{
	public:
		PostProcess(int width, int height, TextureManager* textureManager);
		virtual ~PostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const = 0;

		void BindInternalTextureAsOutput() const { _textureManager->BindImageTexture(0, _outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F); }
		Texture2D* GetProcessedResult() { return _outputTexture; }

		void SetInputTexture(Texture2D* inputTexture) { _inputTexture = inputTexture; };

	protected:
		Texture2D* _outputTexture;
		Texture2D* _inputTexture;

	private:
		TextureManager* _textureManager;
	};
}