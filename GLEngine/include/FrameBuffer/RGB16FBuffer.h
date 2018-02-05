#pragma once
#include <FrameBuffer\FrameBuffer.h>
#include <Texture\Texture2DRGB16F.h>

namespace GLEngine
{
	class RGB16FBuffer :
		public FrameBuffer
	{
	public:
		RGB16FBuffer(int width, int height, FrameBufferManager* manager);
		RGB16FBuffer(int width, int height, GLuint depthBuffer, FrameBufferManager* manager);
		~RGB16FBuffer();

		Texture2D* GetBoundTexture() const { return _boundTexture; }

	private:
		// Lighting texture.
		Texture2DRGB16F * _boundTexture;
	};

}
