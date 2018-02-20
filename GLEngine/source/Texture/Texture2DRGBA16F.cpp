#include <stdafx.h>
#include <Texture\Texture2DRGBA16F.h>


namespace GLEngine
{
	Texture2DRGBA16F::Texture2DRGBA16F(string path)
		: Texture2D(path)
	{
	}

	Texture2DRGBA16F::Texture2DRGBA16F(int width, int height, const void * data)
		: Texture2D(width, height, data)
	{
	}

	Texture2DRGBA16F::Texture2DRGBA16F(GLuint id, int width, int height)
		: Texture2D(id, width, height)
	{
	}

	Texture2DRGBA16F::~Texture2DRGBA16F()
	{
	}

	void Texture2DRGBA16F::SetData(const void* data)
	{
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2DRGBA16F::Generate(int width, int height, const void * data)
	{
		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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