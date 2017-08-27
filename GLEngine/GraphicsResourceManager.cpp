#include "stdafx.h"
#include "GraphicsResourceManager.h"

namespace GLEngine
{
	GraphicsResourceManager::GraphicsResourceManager(int viewportWidth, int viewportHeight, TextureManager* textureManager)
		: _viewportWidth(viewportWidth), _viewportHeight(viewportHeight), _textureManager(textureManager)
	{
		_loadedShaders = map<string, ShaderProgram*>();
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{
	}

	ShaderProgram * GraphicsResourceManager::GetShader(string vertexShader, string fragmentShader)
	{
		map<string, ShaderProgram*>::iterator foundShader = _loadedShaders.find(vertexShader + fragmentShader);

		if (foundShader == _loadedShaders.end())
		{
			ShaderProgram* newShader = new ShaderProgram(vertexShader, fragmentShader);
			_loadedShaders.insert(std::pair<string, ShaderProgram*>(vertexShader + fragmentShader, newShader));

			return newShader;
		}

		return (*foundShader).second;
	}

	ShaderProgram * GraphicsResourceManager::GetModelPBRShader()
	{
		return GetShader("VertexShader.vert", "ModelShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetModelPBRInstancedShader()
	{
		return GetShader("InstancedVertexShader.vert", "ModelShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapShader()
	{
		return GetShader("VertexShader.vert", "EnvironmentMapFragmentShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapLightShader()
	{
		return GetShader("ScreenSpace.vert", "EnvironmentMapLight.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetPbrCombinerShader()
	{
		return GetShader("ScreenSpace.vert", "PBRCombiner.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapConvolutionShader()
	{
		return GetShader("ScreenSpace.vert", "EnvironmentMapConvolution.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetTextureDrawShader()
	{
		return GetShader("ScreenSpace.vert", "DrawTexture.frag");
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