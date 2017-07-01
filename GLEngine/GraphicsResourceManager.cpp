#include "stdafx.h"
#include "GraphicsResourceManager.h"


namespace GLEngine
{
	GraphicsResourceManager::GraphicsResourceManager(TextureManager* textureManager)
		: _textureManager(textureManager)
	{
	}


	GraphicsResourceManager::~GraphicsResourceManager()
	{
	}

	ShaderProgram * GraphicsResourceManager::GetLambertianShader()
	{
		if (_lambertianShader == nullptr)
		{
			_lambertianShader = new ShaderProgram("VertexShader.txt", "FragmentShader.txt");
		}

		return _lambertianShader;
	}

	ShaderProgram * GraphicsResourceManager::GetLambertianInstancedShader()
	{
		if (_lambertianInstancedShader == nullptr)
		{
			_lambertianInstancedShader = new ShaderProgram("InstancedVertexShader.txt", "FragmentShader.txt");
		}

		return _lambertianInstancedShader;
	}

}