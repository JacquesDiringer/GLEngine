#pragma once
#include <Texture\Texture2D.h>

namespace GLEngine
{
	class Texture2DRGB16F :
		public Texture2D
	{
	public:
		Texture2DRGB16F(string path);
		Texture2DRGB16F(int width, int height, unsigned char* data);
		Texture2DRGB16F(GLuint id, int width, int height);
		~Texture2DRGB16F();

		// Sets the pixel data for the texture.
		// Should be called prior to any other texture assignation to units, since it will change the bound texture.
		virtual void SetData(unsigned char* data);

	protected:
		virtual void Generate(int width, int height, unsigned char* data);
	};


}