#include "stdafx.h"
#include "Texture2D.h"

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

		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Generate mip maps.
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free SOIL memory
		SOIL_free_image_data(image);

		_id = texture;
		_path = path;

		_width = imageWidth;
		_height = imageHeight;
	}

	Texture2D::Texture2D(GLuint id, int width, int height)
		: _id(id), _width(width), _height(height)
	{
		_boundUnit = -1;
		_path = "";
	}

	Texture2D::~Texture2D()
	{
	}

	const void Texture2D::BindToUnit(const GLint unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, _id);

		_boundUnit = unit;
	}

	const void Texture2D::UnbindFromUnit()
	{
		glActiveTexture(GL_TEXTURE0 + _boundUnit);	// Activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, 0);

		_boundUnit = -1;
	}

	const bool Texture2D::operator==(const Texture2D & otherTexture)
	{
		return _id == otherTexture.GetId() && _path == otherTexture.GetPath();
	}
}