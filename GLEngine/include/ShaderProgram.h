#pragma once

#include <string>
#include <unordered_map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <Uniform.h>
#include <Texture\TextureManager.h>

using std::string;
using std::unordered_map;

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
		unordered_map<string, Uniform*> _uniforms;
	};
}