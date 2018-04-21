#pragma once

#include <unordered_map>

#include <ShaderProgram.h>
#include <ComputeShaderProgram.h>
#include <VertexArrayObject.h>
#include <Mesh\OBJLoader.h>
#include <FrameBuffer\FrameBufferManager.h>
#include <GraphicsDeviceManager.h>
#include <VBOManager.h>

using std::unordered_map;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API GraphicsResourceManager
	{
	public:
		GraphicsResourceManager(int viewportWidth, int viewportHeight);
		virtual ~GraphicsResourceManager();

		int GetViewportWidth() const { return _viewportWidth; }
		int GetViewportHeight() const { return _viewportHeight; }

		TextureManager* GetTextureManager() { return _textureManager; }

		FrameBufferManager* GetFrameBufferManager() { return _frameBufferManager; }

		GraphicsDeviceManager* GetGraphicsDeviceManager() { return _graphicsDeviceManager; }

		VBOManager* GetVBOManager() { return _vboManager; }

		ShaderProgram* GetShader(string vertexShader, string fragmentShader);
		ShaderProgram* GetModelPBRShader();
		ShaderProgram* GetModelPBRInstancedShader();
		ShaderProgram* GetEnvmapShader();
		ShaderProgram* GetEnvmapLightShader();
		ShaderProgram* GetPbrCombinerShader();

		ShaderProgram* GetEnvmapConvolutionShader();

		ShaderProgram* GetTextureDrawShader();

		ComputeShaderProgram* GetComputeShader(string computeShaderPath);

		VertexArrayObject* GetScreenVAO();
		VertexArrayObject* GetSphereVAO();

	private:
		int _viewportWidth, _viewportHeight;

		TextureManager* _textureManager;

		FrameBufferManager* _frameBufferManager;

		GraphicsDeviceManager* _graphicsDeviceManager;

		VBOManager* _vboManager;

		OBJLoader* _objLoader;

		VertexArrayObject* _screenVAO = nullptr;
		VertexArrayObject* _sphereVAO = nullptr;

		unordered_map<string, ShaderProgram*> _loadedShaders;
	};

}
