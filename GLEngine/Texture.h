#pragma once

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

namespace GLEngine
{
	class Texture
	{
	public:
		virtual ~Texture();

		const GLuint GetId() const { return _id; }
		const GLint GetBoundUnit() const { return _boundUnit; }

		// Binds the teture to te specified unit while keeping track of the bound unit.
		virtual const void BindToUnit(const GLint unit)=0;
		// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
		virtual const void UnbindFromUnit()=0;

		const bool operator==(const Texture& otherTexture);

	protected:
		GLuint _id;
		GLint _boundUnit;
	};

}