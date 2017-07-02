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
			_lambertianShader = new ShaderProgram("VertexShader.vert", "ModelShader.frag");
		}

		return _lambertianShader;
	}

	ShaderProgram * GraphicsResourceManager::GetLambertianInstancedShader()
	{
		if (_lambertianInstancedShader == nullptr)
		{
			_lambertianInstancedShader = new ShaderProgram("InstancedVertexShader.vert", "ModelShader.frag");
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

	ShaderProgram * GraphicsResourceManager::GetEnvmapLightShader()
	{
		if (_envmapLightShader == nullptr)
		{
			_envmapLightShader = new ShaderProgram("ScreenSpace.vert", "EnvironmentMapLight.frag");
		}

		return _envmapLightShader;
	}

	ShaderProgram * GraphicsResourceManager::GetPbrCombinerShader()
	{
		if (_pbrCombinerShader == nullptr)
		{
			_pbrCombinerShader = new ShaderProgram("ScreenSpace.vert", "PBRCombiner.frag");
		}

		return _pbrCombinerShader;
	}

	VertexArrayObject * GraphicsResourceManager::GetScreenVAO()
	{
		if (_screenVAO == nullptr)
		{
			list<int> elements = list<int>{	0, 1, 3,
											1, 2, 3};

			list<Vector3*> positions = list<Vector3*>{	new Vector3(-1, -1, -1), 
														new Vector3(1, -1, -1),
														new Vector3(1, 1, -1), 
														new Vector3(-1, 1, -1) };

			list<Vector2*> textureCoordinates = list<Vector2*>{	new Vector2(0, 0),
																new Vector2(1, 0),
																new Vector2(1, 1),
																new Vector2(0, 1) };

			_screenVAO = new VertexArrayObject(elements, positions, textureCoordinates);
		}

		return _screenVAO;
	}

}