#include "stdafx.h"
#include "GraphicsResourceManager.h"

namespace GLEngine
{
	GraphicsResourceManager::GraphicsResourceManager(int viewportWidth, int viewportHeight, TextureManager* textureManager)
		: _viewportWidth(viewportWidth), _viewportHeight(viewportHeight), _textureManager(textureManager)
	{
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{
	}

	ShaderProgram * GraphicsResourceManager::GetModelPBRShader()
	{
		if (_modelPBRShader == nullptr)
		{
			_modelPBRShader = new ShaderProgram("VertexShader.vert", "ModelShader.frag");
		}

		return _modelPBRShader;
	}

	ShaderProgram * GraphicsResourceManager::GetModelPBRInstancedShader()
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

	ShaderProgram * GraphicsResourceManager::GetEnvmapConvolutionShader()
	{
		if (_envmapConvolutionShader == nullptr)
		{
			_envmapConvolutionShader = new ShaderProgram("ScreenSpace.vert", "EnvironmentMapConvolution.frag");
		}

		return _envmapConvolutionShader;
	}

	ShaderProgram * GraphicsResourceManager::GetTextureDrawShader()
	{
		if (_textureDrawShader == nullptr)
		{
			_textureDrawShader = new ShaderProgram("ScreenSpace.vert", "DrawTexture.frag");
		}

		return _textureDrawShader;
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