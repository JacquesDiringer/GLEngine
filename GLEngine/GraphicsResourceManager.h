#pragma once

#include <map>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "OBJLoader.h"
#include "FrameBufferManager.h"

using std::map;

namespace GLEngine
{
	class GraphicsResourceManager
	{
	public:
		GraphicsResourceManager(int viewportWidth, int viewportHeight);
		virtual ~GraphicsResourceManager();

		int GetViewportWidth() const { return _viewportWidth; }
		int GetViewportHeight() const { return _viewportHeight; }

		TextureManager* GetTextureManager() { return _textureManager; }

		FrameBufferManager* GetFrameBufferManager() { return _frameBufferManager; }

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

		FrameBufferManager* _frameBufferManager;

		OBJLoader* _objLoader;

		VertexArrayObject* _screenVAO = nullptr;
		VertexArrayObject* _sphereVAO = nullptr;

		map<string, ShaderProgram*> _loadedShaders;
	};

}
