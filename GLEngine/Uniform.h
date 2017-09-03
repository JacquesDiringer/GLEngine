#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Maths
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

using std::string;

using Math::Vector2;
using Math::Vector3;
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
		void SetValue(const Vector2* vector);
		void SetValue(const Vector3* vector);
		void SetValue(const Matrix4* matrix);
		void SetValue(const Matrix4 matrix);

	private:
		string _name;
		GLuint _location;
		GLenum _type;
	};
}