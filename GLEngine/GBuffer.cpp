#include "stdafx.h"
#include "GBuffer.h"


namespace GLEngine
{
	GBuffer::GBuffer(int width, int height)
		: FrameBuffer(width, height)
	{
		// G-Buffer creation.
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		// First bind a depth buffer.
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


		// This texture will hold the (compressed) normal on RGB
		// The depth on the alpha.
		GLuint geometryTextureId;
		glGenTextures(1, &geometryTextureId);
		glBindTexture(GL_TEXTURE_2D, geometryTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, geometryTextureId, 0);
		_geometryTexture = new Texture2D(geometryTextureId, width, height);

		// This texture will hold the diffuse color on RGB
		// No alpha yet.
		GLuint diffuseTextureId;
		glGenTextures(1, &diffuseTextureId);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 1.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuseTextureId, 0);
		_diffuseTexture = new Texture2D(diffuseTextureId, width, height);

		// This texture will hold the specular color on RGB
		// The roughness on the alpha.
		GLuint specularRoughnessTextureId;
		glGenTextures(1, &specularRoughnessTextureId);
		glBindTexture(GL_TEXTURE_2D, specularRoughnessTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 2.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specularRoughnessTextureId, 0);
		_specularRoughnessTexture = new Texture2D(specularRoughnessTextureId, width, height);

		// This texture will hold the emissive color on RGB
		// No alpha yet.
		GLuint emissiveTextureId;
		glGenTextures(1, &emissiveTextureId);
		glBindTexture(GL_TEXTURE_2D, emissiveTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 3.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, emissiveTextureId, 0);
		_emissiveTexture = new Texture2D(emissiveTextureId, width, height);

		// Enable drawing in these 4 color attachment
		GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

		// Check if the frame buffer is complete.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw new std::exception("G-Buffer frame buffer incomplete.");
		}
	}


	GBuffer::~GBuffer()
	{
	}

}