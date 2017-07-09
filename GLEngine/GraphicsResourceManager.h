#pragma once

#include "ShaderProgram.h"
#include "VertexArrayObject.h"

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
		ShaderProgram* GetModelPBRShader();
		ShaderProgram* GetLambertianInstancedShader();
		ShaderProgram* GetEnvmapShader();
		ShaderProgram* GetEnvmapLightShader();
		ShaderProgram* GetPbrCombinerShader();

		ShaderProgram* GetEnvmapConvolutionShader();

		ShaderProgram* GetTextureDrawShader();

		VertexArrayObject* GetScreenVAO();

	private:
		int _viewportWidth, _viewportHeight;

		TextureManager* _textureManager;
		ShaderProgram* _modelPBRShader = nullptr;
		ShaderProgram* _lambertianInstancedShader = nullptr;
		ShaderProgram* _envmapShader = nullptr;
		ShaderProgram* _envmapLightShader = nullptr;
		ShaderProgram* _pbrCombinerShader = nullptr;

		ShaderProgram* _envmapConvolutionShader = nullptr;

		ShaderProgram* _textureDrawShader = nullptr;

		VertexArrayObject* _screenVAO = nullptr;
	};

}
