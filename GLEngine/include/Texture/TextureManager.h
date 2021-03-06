#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <Texture\Texture.h>
#include <Texture\Texture2D.h>

using std::string;
using std::vector;
using std::unordered_map;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		// Loads a texture or gets it from the library if already loaded.
		Texture2D* GetTexture(const string texPath);

		// If the texture is not already assigned to a unit, tries to find a free unit and assign the texture to it.
		// Returns the id of the assigned unit.
		// Returns -1 when no unit could be found to store the texture (ie. every unit is already taken).
		const GLuint AssignTextureToUnit(Texture* texture);

		// Unbind textures from every available units.
		const void FreeUnits();

		void BindImageTexture(GLint imageUnit, Texture* texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

	private:
		// Textures2D are the only ones that are loaded from disk, therefore the only ones we are keeping a track of for performance purposes.
		unordered_map<string, Texture2D*> _loadedTexture2DLibrary;
		vector <Texture*> _textureUnits;
		vector <Texture*> _imageTextureUnits;
		GLint _maxTextureUnits;
	};
}