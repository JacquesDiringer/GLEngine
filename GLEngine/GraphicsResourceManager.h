#pragma once

#include <map>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "OBJLoader.h"

using std::map;

namespace GLEngine
{
	class GraphicsResourceManager
	{
	public:
		GraphicsResourceManager(int viewportWidth, int viewportHeight, TextureManager* textureManager);
		virtual ~GraphicsResourceManager();

		int GetViewportWidth() const { return _viewportWidth; }
		int GetViewportHeight() const { return _viewportHeight; }

		TextureManager* GetTextureManager() { return _textureManager; }

		ShaderProgram* GetShader(string vertexShader, string fragmentShader);
		ShaderProgram* GetModelPBRShader();
		ShaderProgram* GetModelPBRInstancedShader();
		ShaderProgram* GetEnvmapShader();
		ShaderProgram* GetEnvmapLightShader();
		ShaderProgram* GetPbrCombinerShader();

		ShaderProgram* GetEnvmapConvolutionShader();

		ShaderProgram* GetTextureDrawShader();

		VertexArrayObject* GetScreenVAO();
		VertexArrayObject* GetSphereVAO();

	private:
		int _viewportWidth, _viewportHeight;

		TextureManager* _textureManager;

		OBJLoader* _objLoader;

		VertexArrayObject* _screenVAO = nullptr;
		VertexArrayObject* _sphereVAO = nullptr;

		map<string, ShaderProgram*> _loadedShaders;
	};

}
