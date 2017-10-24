#include "stdafx.h"
#include "Texture2DRGB16F.h"


namespace GLEngine
{
	Texture2DRGB16F::Texture2DRGB16F(string path)
		: Texture2D(path)
	{
	}

	Texture2DRGB16F::Texture2DRGB16F(int width, int height, unsigned char * data)
		: Texture2D(width, height, data)
	{
	}

	Texture2DRGB16F::Texture2DRGB16F(GLuint id, int width, int height)
		: Texture2D(id, width, height)
	{
	}

	Texture2DRGB16F::~Texture2DRGB16F()
	{
	}

	void Texture2DRGB16F::SetData(unsigned char * data)
	{
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2DRGB16F::Generate(int width, int height, unsigned char * data)
	{
		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Generate mip maps.
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}