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
			_lambertianShader = new ShaderProgram("VertexShader.vert", "FragmentShader.frag");
		}

		return _lambertianShader;
	}

	ShaderProgram * GraphicsResourceManager::GetLambertianInstancedShader()
	{
		if (_lambertianInstancedShader == nullptr)
		{
			_lambertianInstancedShader = new ShaderProgram("InstancedVertexShader.vert", "FragmentShader.frag");
		}

		return _lambertianInstancedShader;
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapShader()
	{
		if (_envmapShader == nullptr)
		{
			_envmapShader = new ShaderProgram("VertexShader.vert", "EnvironmentMapFragmentShader.frag");
		}

		return _envmapShader;
	}

}