#pragma once
#include <Texture\Texture.h>

#include<string>

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

#include <SOIL.h>

using std::string;

namespace GLEngine
{
	class Texture2D :
		public Texture
	{
	public:
		Texture2D(string path);
		Texture2D(int width, int height, const void* data);
		Texture2D(GLuint id, int width, int height);
		virtual ~Texture2D();

		const string GetPath() const { return _path; }

		const int GetWidth() const { return _width; }
		const int GetHeight() const { return _height; }

		// Binds the teture to te specified unit while keeping track of the bound unit.
		const void BindToUnit(const GLint textureUnit);
		// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
		const void UnbindFromUnit();

		virtual void SetFiltering(GLenum minFilter, GLenum magFilter);

		// Generates mip maps.
		void GenerateMipMaps();

		// Sets the pixel data for the texture.
		// Should be called prior to any other texture assignation to units, since it will change the bound texture.
		virtual void SetData(const void* data) = 0;

		const bool operator==(const Texture2D& otherTexture);

	protected:
		string _path;
		int _width, _height;

		virtual void Generate(int width, int height, const void* data);
	};
}