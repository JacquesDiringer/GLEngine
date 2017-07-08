#pragma once
#include "Texture.h"

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
		Texture2D(GLuint id, int width, int height);
		~Texture2D();

		const string GetPath() const { return _path; }

		const int GetWidth() const { return _width; }
		const int GetHeight() const { return _height; }

		// Binds the teture to te specified unit while keeping track of the bound unit.
		const void BindToUnit(const GLint unit);
		// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
		const void UnbindFromUnit();

		const bool operator==(const Texture2D& otherTexture);

	private:
		string _path;
		int _width, _height;
	};
}