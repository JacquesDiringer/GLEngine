#include <stdafx.h>
#include <Texture\Texture.h>


namespace GLEngine
{
	Texture::~Texture()
	{
	}

	const bool Texture::operator==(const Texture & otherTexture)
	{
		return _id == otherTexture.GetId();
	}

	void Texture::BindImageTexture(GLint imageUnit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
	{
		glBindImageTexture(
			imageUnit,
			_id,
			level,
			layered,
			layer,
			access,
			format);
	}

}