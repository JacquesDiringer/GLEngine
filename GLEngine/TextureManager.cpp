#include "stdafx.h"
#include "TextureManager.h"

namespace GLEngine
{
	TextureManager::TextureManager()
	{
		_loadedTexture2DLibrary = map<string, Texture2D*>();

		// Get the maximum possible numbre of texture units made available by the hardware.
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);

		// And set the vector size accordingly.
		_textureUnits = vector<Texture*>();
		for (int unitIndex = 0; unitIndex < _maxTextureUnits; unitIndex++)
		{
			_textureUnits.push_back(NULL);
		}
	}

	TextureManager::~TextureManager()
	{
	}

	Texture2D * TextureManager::GetTexture(const string texPath)
	{
		map<string, Texture2D*>::iterator findIterator = _loadedTexture2DLibrary.find(texPath);

		// If the texture has already been loaded.
		if (findIterator != _loadedTexture2DLibrary.end())
		{
			return (*findIterator).second;
		}
		else
		{
			// Create the texture.
			Texture2D* result = new Texture2D(texPath);

			// Store it in the dictionary.
			_loadedTexture2DLibrary.insert(std::pair<string, Texture2D*>(texPath, result));

			// Return it.
			return result;
		}
	}

	const GLuint TextureManager::AssignTextureToUnit(Texture * texture)
	{
		GLuint assignedUnit = -1;

		// Try to find a unit where the texture is already assigned.
		for (GLint unitId = 0; unitId < _maxTextureUnits; unitId++)
		{
			if (_textureUnits[unitId] != NULL && *_textureUnits[unitId] == *texture)
			{
				// Return it.
				return unitId;
			}
		}

		// Try to find a non assigned unit in the units vector.
		for (GLint unitId = 0; unitId < _maxTextureUnits; unitId++)
		{
			if (_textureUnits[unitId] == NULL)
			{
				// Actually bind the texture to the unit.
				texture->BindToUnit(unitId);

				// Store the texture in the unit slot.
				_textureUnits[unitId] = texture;
				return unitId;
			}
		}

		throw new std::exception("There is no more free texture unit to bind the texture to.");
	}

	const void TextureManager::FreeUnits()
	{
		for (GLint unitId = 0; unitId < _maxTextureUnits; unitId++)
		{
			// If this unit is bound to a texture.
			if (_textureUnits[unitId] != NULL)
			{
				// Tells OpenGL to unbind the texture id from the unit while also setting the texture's internal state to "not bound".
				_textureUnits[unitId]->UnbindFromUnit();

				// Free the unit slot, so that the manager knows it is available.
				_textureUnits[unitId] = NULL;
			}
		}
	}
}