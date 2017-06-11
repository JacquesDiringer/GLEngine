#pragma once

#include <string>
#include <vector>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Texture2D.h"

using std::string;
using std::vector;
using std::map;

namespace GLEngine
{
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		// Loads a texture or gets it from the library if already loaded.
		Texture2D* GetTexture(const string texPath);

		// If the texture is not already assigned to a unit, tries to find a free unit and assign the texture to it.
		// Returns the id of the assigned unit.
		// Returns -1 when no unit could be found to store the texture (ie. every unit is already taken).
		const GLuint AssignTextureToUnit(Texture2D* texture);

		// Unbind textures from every available units.
		const void FreeUnits();

	private:
		map<string, Texture2D*> _textureLibrary;
		vector <Texture2D*> _textureUnits;
		GLint _maxTextureUnits;
	};
}