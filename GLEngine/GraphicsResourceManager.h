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
		ShaderProgram* GetLambertianShader();
		ShaderProgram* GetLambertianInstancedShader();
		ShaderProgram* GetEnvmapShader();
		ShaderProgram* GetEnvmapLightShader();
		ShaderProgram* GetPbrCombinerShader();

		VertexArrayObject* GetScreenVAO();

	private:
		TextureManager* _textureManager;
		ShaderProgram* _lambertianShader = nullptr;
		ShaderProgram* _lambertianInstancedShader = nullptr;
		ShaderProgram* _envmapShader = nullptr;
		ShaderProgram* _envmapLightShader = nullptr;
		ShaderProgram* _pbrCombinerShader = nullptr;

		VertexArrayObject* _screenVAO = nullptr;
	};

}
