#pragma once

#include "ShaderProgram.h"

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

	private:
		TextureManager* _textureManager;
		ShaderProgram* _lambertianShader = nullptr;
		ShaderProgram* _lambertianInstancedShader = nullptr;

		ShaderProgram* _envmapShader = nullptr;
	};

}
