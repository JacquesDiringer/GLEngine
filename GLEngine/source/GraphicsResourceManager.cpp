#include <stdafx.h>
#include "GraphicsResourceManager.h"

#include <Mesh\OBJMesh.h>

namespace GLEngine
{
	GraphicsResourceManager::GraphicsResourceManager(int viewportWidth, int viewportHeight)
		: _viewportWidth(viewportWidth), _viewportHeight(viewportHeight)
	{
		_textureManager = new TextureManager();
		_frameBufferManager = new FrameBufferManager();
		_graphicsDeviceManager = new GraphicsDeviceManager();
		_vboManager = new VBOManager();
		_loadedShaders = unordered_map<string, ShaderProgram*>();
		_objLoader = new OBJLoader();
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{
	}

	ShaderProgram * GraphicsResourceManager::GetShader(string vertexShader, string fragmentShader)
	{
		auto foundShader = _loadedShaders.find(vertexShader + fragmentShader);

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
		return GetShader("..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\VertexShader.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\ModelShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetModelPBRInstancedShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\InstancedVertexShader.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\ModelShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\VertexShader.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\GBuffer\\EnvironmentMapFragmentShader.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapLightShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\Lighting\\EnvironmentMapLight.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetPbrCombinerShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\Combiner\\PBRCombiner.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetEnvmapConvolutionShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\ImageProcessing\\EnvironmentMapConvolution.frag");
	}

	ShaderProgram * GraphicsResourceManager::GetTextureDrawShader()
	{
		return GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\Common\\DrawTexture.frag");
	}

	ComputeShaderProgram* GraphicsResourceManager::GetComputeShader(string computeShaderPath)
	{
		auto foundShader = _loadedShaders.find(computeShaderPath);

		if (foundShader == _loadedShaders.end())
		{
			ComputeShaderProgram* newShader = new ComputeShaderProgram(computeShaderPath);
			_loadedShaders.insert(std::pair<string, ShaderProgram*>(computeShaderPath, newShader));

			return newShader;
		}

		return dynamic_cast<ComputeShaderProgram*>((*foundShader).second);
	}

	VertexArrayObject * GraphicsResourceManager::GetScreenVAO()
	{
		if (_screenVAO == nullptr)
		{
			vector<int> elements = vector<int>{	0, 1, 3,
											1, 2, 3};

			vector<GLEngineMath::Vector3*> positions = vector<GLEngineMath::Vector3*>{	new GLEngineMath::Vector3(-1, -1, -1),
														new GLEngineMath::Vector3(1, -1, -1),
														new GLEngineMath::Vector3(1, 1, -1), 
														new GLEngineMath::Vector3(-1, 1, -1) };

			vector<GLEngineMath::Vector2*> textureCoordinates = vector<GLEngineMath::Vector2*>{	new GLEngineMath::Vector2(0, 0),
																new GLEngineMath::Vector2(1, 0),
																new GLEngineMath::Vector2(1, 1),
																new GLEngineMath::Vector2(0, 1) };

			_screenVAO = new VertexArrayObject(elements, positions, textureCoordinates);
		}

		return _screenVAO;
	}

	VertexArrayObject * GraphicsResourceManager::GetSphereVAO()
	{
		if (_sphereVAO == nullptr)
		{
			OBJMesh* sphereMesh = (OBJMesh*)_objLoader->LoadModel("..\\GLEngine\\resource\\Meshes\\sphere_UVs.obj");
			sphereMesh->InitializeVao();

			_sphereVAO = sphereMesh->GetVao();
		}

		return _sphereVAO;
	}
}