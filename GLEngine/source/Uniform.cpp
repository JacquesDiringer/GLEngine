#include <stdafx.h>
#include "Uniform.h"


namespace GLEngine
{
	Uniform::Uniform(string name, GLuint location, GLenum type)
		: _name(name), _location(location), _type(type)
	{
		// void
	}

	Uniform::~Uniform()
	{
	}

	void Exception()
	{
		throw new std::invalid_argument("Type does not correspond to the shader uniform type.");
	}

	void Uniform::SetValue(GLuint unsignedIntegerValue)
	{
		if (_type == GL_UNSIGNED_INT)
		{
			glUniform1ui(_location, unsignedIntegerValue);
		}
		else if (_type == GL_SAMPLER_2D)
		{
			glUniform1i(_location, (GLint)unsignedIntegerValue);
		}
		else if (_type == GL_SAMPLER_3D)
		{
			glUniform1i(_location, (GLint)unsignedIntegerValue);
		}
		else
		{
			Exception();
		}
	}

	void Uniform::SetValue(GLint integerValue)
	{
		if (_type == GL_INT || _type == GL_SAMPLER_2D || _type == GL_SAMPLER_3D)
		{
			glUniform1i(_location, integerValue);
		}
		else
		{
			Exception();
		}
	}

	void Uniform::SetValue(GLfloat floatingValue)
	{
		if (_type == GL_FLOAT)
		{
			glUniform1f(_location, floatingValue);
		}
		else
		{
			Exception();
		}
	}

	void Uniform::SetValue(const Vector2& vector)
	{
		if (_type == GL_FLOAT_VEC2)
		{
			glUniform2f(_location, vector.X(), vector.Y());
		}
		else
		{
			Exception();
		}
	}

	void Uniform::SetValue(const Vector3& vector)
	{
		if (_type == GL_FLOAT_VEC3)
		{
			glUniform3f(_location, vector.X(), vector.Y(), vector.Z());
		}
		else
		{
			Exception();
		}
	}

	void Uniform::SetValue(const Matrix4& matrix)
	{
		if (_type == GL_FLOAT_MAT4)
		{
			GLfloat* matArray = matrix.GetArray();
			
			glUniformMatrix4fv(_location, 1, GL_TRUE, matArray);

			delete matArray;
		}
		else
		{
			Exception();
		}
	}
}