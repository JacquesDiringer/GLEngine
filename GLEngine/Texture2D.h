#pragma once

#include<string>

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

#include <SOIL.h>

using std::string;

namespace GLEngine
{
	class Texture2D
	{
	public:
		Texture2D(string path);
		~Texture2D();

		const GLuint GetId() const { return _id; }
		const string GetPath() const { return _path; }
		const GLint GetBoundUnit() const { return _boundUnit; }

		// Binds the teture to te specified unit while keeping track of the bound unit.
		const void BindToUnit(const GLint unit);
		// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
		const void UnbindFromUnit();

		const bool operator==(const Texture2D& otherTexture);

	private:
		GLuint _id;
		string _path;
		GLint _boundUnit;
	};
}