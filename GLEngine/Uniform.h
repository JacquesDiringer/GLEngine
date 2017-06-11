#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Maths
#include "Matrix4.h"

using std::string;

using Math::Matrix4;


namespace GLEngine
{
	class Uniform
	{
	public:
		Uniform(string name, GLuint location, GLenum type);
		~Uniform();

		// The following will throw an invalid_argument exception if the argument's type does not correspond to the uniform type in the shader.
		void SetValue(GLuint unsignedIntegerValue);
		void SetValue(GLint integerValue);
		void SetValue(GLfloat floatingValue);
		void SetValue(const Matrix4* matrix);

	private:
		string _name;
		GLuint _location;
		GLenum _type;
	};
}