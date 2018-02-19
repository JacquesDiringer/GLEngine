#include <stdafx.h>
#include <Texture\Texture2D.h>

namespace GLEngine
{
	Texture2D::Texture2D(string path)
	{
		_boundUnit = -1;

		int imageWidth, imageHeight;
		unsigned char* image = SOIL_load_image(path.c_str(), &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

		if (image == NULL)
		{
			throw new std::invalid_argument("Invalid image path.");
		}

		// Call the data constructor with the just loaded data.
		Generate(imageWidth, imageHeight, image);

		// Free SOIL memory
		SOIL_free_image_data(image);

		_path = path;
	}

	Texture2D::Texture2D(int width, int height, const void* data)
	{
		Generate(width, height, data);
	}

	Texture2D::Texture2D(GLuint id, int width, int height)
		: _width(width), _height(height)
	{
		_id = id;
		_boundUnit = -1;
		_path = "";
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &_id);
	}

	const void Texture2D::BindToUnit(const GLint textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, _id);

		_boundUnit = textureUnit;
	}

	const void Texture2D::UnbindFromUnit()
	{
		glActiveTexture(GL_TEXTURE0 + _boundUnit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, 0);

		_boundUnit = -1;
	}

	void Texture2D::SetFiltering(GLenum minFilter, GLenum magFilter)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		}
		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::GenerateMipMaps()
	{
		glGenerateTextureMipmap(_id);
	}

	const bool Texture2D::operator==(const Texture2D & otherTexture)
	{
		return _id == otherTexture.GetId() && _path == otherTexture.GetPath();
	}
	void Texture2D::Generate(int width, int height, const void * data)
	{
		_width = width;
		_height = height;

		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// Generate mip maps.
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}