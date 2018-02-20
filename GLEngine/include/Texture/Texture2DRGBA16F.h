#pragma once
#include <Texture\Texture2D.h>

namespace GLEngine
{
	class Texture2DRGBA16F :
		public Texture2D
	{
	public:
		Texture2DRGBA16F(string path);
		Texture2DRGBA16F(int width, int height, const void* data);
		Texture2DRGBA16F(GLuint id, int width, int height);
		~Texture2DRGBA16F();

		// Sets the pixel data for the texture.
		// Should be called prior to any other texture assignation to units, since it will change the bound texture.
		virtual void SetData(const void* data);

	protected:
		virtual void Generate(int width, int height, const void* data);
	};


}