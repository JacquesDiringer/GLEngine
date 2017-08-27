#pragma once
#include "Renderable.h"
#include "RGB16FBuffer.h"

namespace GLEngine
{
	class PostProcess
		: public Renderable
	{
	public:
		PostProcess(int width, int height);
		virtual ~PostProcess();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) = 0;

		void BindFrameBuffer() { _processedFrameBuffer->Bind(); }
		Texture2D* GetProcessedResult() { return _processedFrameBuffer->GetBoundTexture(); }

		void SetInputTexture(Texture2D* inputTexture) { _inputTexture = inputTexture; };

	protected:
		RGB16FBuffer* _processedFrameBuffer;
		Texture2D* _inputTexture;
	};
}