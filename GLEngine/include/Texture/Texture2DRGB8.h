#pragma once
#include <Texture\Texture2D.h>

namespace GLEngine
{
	class Texture2DRGB8 :
		public Texture2D
	{
	public:
		Texture2DRGB8(string path);
		Texture2DRGB8(int width, int height, const void* data);
		Texture2DRGB8(GLuint id, int width, int height);
		~Texture2DRGB8();

		// Sets the pixel data for the texture.
		// Should be called prior to any other texture assignation to units, since it will change the bound texture.
		virtual void SetData(const void* data);

	protected:
		virtual void Generate(int width, int height, const void* data);
	};


}