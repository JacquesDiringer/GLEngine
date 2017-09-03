#include "stdafx.h"
#include "RGB16FBuffer.h"


namespace GLEngine
{
	RGB16FBuffer::RGB16FBuffer(int width, int height)
		: RGB16FBuffer(width, height, 0)
	{
	}

	RGB16FBuffer::RGB16FBuffer(int width, int height, GLuint depthBuffer)
		: FrameBuffer(width, height)
	{
		// Lighting frame buffer creation.
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		if (depthBuffer != 0)
		{
			// First bind a depth buffer.
			glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		}

		// This texture will hold the lighting value on RGB
		// No alpha yet.
		GLuint lightingTextureId;
		glGenTextures(1, &lightingTextureId);
		glBindTexture(GL_TEXTURE_2D, lightingTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingTextureId, 0);
		_boundTexture = new Texture2D(lightingTextureId, width, height);

		// Enable drawing in the lighting texture.
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Check if the frame buffer is complete.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw new std::exception("G-Buffer frame buffer incomplete.");
		}
	}


	RGB16FBuffer::~RGB16FBuffer()
	{
	}

}