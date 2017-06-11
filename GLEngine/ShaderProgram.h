#pragma once

#include <string>
#include <list>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Uniform.h"
#include "TextureManager.h"

using std::string;
using std::list;
using std::map;

namespace GLEngine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(string vertexShaderFile, string fragmentShaderFile, TextureManager* textureManager);
		~ShaderProgram();

		void Use();

		// Accessors
		Uniform* GetUniform(string name);

	private:
		GLuint _shaderProgramId;
		map<string, Uniform*> _uniforms;
		TextureManager* _textureManager;
	};
}