#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Maths
#include <Math\Vector2.h>
#include <Math\Vector3.h>
#include <Math\Matrix4.h>

using std::string;

namespace GLEngine
{
	class Uniform
	{
	public:
		Uniform(string name, GLint location, GLenum type);
		~Uniform();

		// The following will throw an invalid_argument exception if the argument's type does not correspond to the uniform type in the shader.
		void SetValue(GLuint unsignedIntegerValue);
		void SetValue(GLint integerValue);
		void SetValue(GLfloat floatingValue);
		void SetValue(const GLEngineMath::Vector2& vector);
		void SetValue(const GLEngineMath::Vector3& vector);
		void SetValue(const GLEngineMath::Matrix4& matrix);

	private:
		string _name;
		GLint _location;
		GLenum _type;
	};
}