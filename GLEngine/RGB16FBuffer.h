#pragma once
#include "FrameBuffer.h"
#include "Texture2D.h"

namespace GLEngine
{
	class RGB16FBuffer :
		public FrameBuffer
	{
	public:
		RGB16FBuffer(int width, int height);
		RGB16FBuffer(int width, int height, GLuint depthBuffer);
		~RGB16FBuffer();

		Texture2D* GetBoundTexture() { return _boundTexture; }

	private:
		// Lighting texture.
		Texture2D * _boundTexture;
	};

}
