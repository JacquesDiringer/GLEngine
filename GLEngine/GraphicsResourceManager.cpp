#include "stdafx.h"
#include "GraphicsResourceManager.h"

#include "OBJMesh.h"

namespace GLEngine
{
	GraphicsResourceManager::GraphicsResourceManager(int viewportWidth, int viewportHeight)
		: _viewportWidth(viewportWidth), _viewportHeight(viewportHeight)
	{
		_textureManager = new TextureManager();
		_frameBufferManager = new FrameBufferManager();
		_graphicsDeviceManager = new GraphicsDeviceManager();
		_loadedShaders = map<string, ShaderProgram*>();
		_objLoader = new OBJLoader();
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
			vector<int> elements = vector<int>{	0, 1, 3,
											1, 2, 3};

			vector<Vector3*> positions = vector<Vector3*>{	new Vector3(-1, -1, -1),
														new Vector3(1, -1, -1),
														new Vector3(1, 1, -1), 
														new Vector3(-1, 1, -1) };

			vector<Vector2*> textureCoordinates = vector<Vector2*>{	new Vector2(0, 0),
																new Vector2(1, 0),
																new Vector2(1, 1),
																new Vector2(0, 1) };

			_screenVAO = new VertexArrayObject(elements, positions, textureCoordinates);
		}

		return _screenVAO;
	}

	VertexArrayObject * GraphicsResourceManager::GetSphereVAO()
	{
		if (_sphereVAO == nullptr)
		{
			OBJMesh* sphereMesh = (OBJMesh*)_objLoader->LoadModel("sphere_UVs.obj");
			sphereMesh->InitializeVao();

			_sphereVAO = sphereMesh->GetVao();
		}

		return _sphereVAO;
	}
}