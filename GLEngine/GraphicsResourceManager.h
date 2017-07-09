#pragma once

#include "ShaderProgram.h"
#include "VertexArrayObject.h"

namespace GLEngine
{
	class GraphicsResourceManager
	{
	public:
		GraphicsResourceManager(TextureManager* textureManager);
		virtual ~GraphicsResourceManager();

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
