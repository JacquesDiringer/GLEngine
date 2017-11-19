#pragma once

#include <string>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Uniform.h"
#include "TextureManager.h"

using std::string;
using std::map;

namespace GLEngine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(string vertexShaderFile, string fragmentShaderFile);
		~ShaderProgram();

		void Use();

		// Accessors
		Uniform* GetUniform(string name);

	private:
		GLuint _shaderProgramId;
		map<string, Uniform*> _uniforms;
	};
}