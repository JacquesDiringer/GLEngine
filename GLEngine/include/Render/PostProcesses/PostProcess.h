#pragma once
#include <Render\Renderable.h>
#include <FrameBuffer\RGB16FBuffer.h>
#include <FrameBuffer\FrameBufferManager.h>

namespace GLEngine
{
	class PostProcess
		: public Renderable
	{
	public:
		PostProcess(int width, int height, FrameBufferManager* frameBufferManager);
		virtual ~PostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const = 0;

		void BindFrameBuffer() { _processedFrameBuffer->Bind(); }
		Texture2D* GetProcessedResult() { return _processedFrameBuffer->GetBoundTexture(); }

		void SetInputTexture(Texture2D* inputTexture) { _inputTexture = inputTexture; };

	protected:
		RGB16FBuffer* _processedFrameBuffer;
		Texture2D* _inputTexture;
	};
}