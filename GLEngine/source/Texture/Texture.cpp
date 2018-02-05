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

}