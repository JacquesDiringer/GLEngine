#pragma once
#include "Texture.h"
#include "Texture2D.h"

#include<list>

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

using std::list;

namespace GLEngine
{
	class Texture3D :
		public Texture
	{
	public:
		Texture3D(GLuint id, int width, int height, int depth);
		// Generates an FP16 RGB 3D texture.
		// Every 2D texture in the list have to be the same size.
		Texture3D(list<Texture2D*> composerList);
		~Texture3D();

		const int GetWidth() const { return _width; }
		const int GetHeight() const { return _height; }
		const int GetDepth() const { return _depth; }

		// Binds the teture to te specified unit while keeping track of the bound unit.
		const void BindToUnit(const GLint unit);
		// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
		const void UnbindFromUnit();

	private:
		int _width, _height, _depth;
	};
}