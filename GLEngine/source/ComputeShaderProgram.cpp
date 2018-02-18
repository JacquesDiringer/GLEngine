#include "stdafx.h"
#include <ComputeShaderProgram.h>
#include <Utils.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


namespace GLEngine
{
	ComputeShaderProgram::ComputeShaderProgram(string computeShaderPath)
		: ShaderProgram()
	{
		// Create and compile the shader itself.
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
		string computeShaderSource = Utils::ReadFile(computeShaderPath);
		const GLchar* computeShaderSourceChar = computeShaderSource.c_str();
		glShaderSource(computeShader, 1, &computeShaderSourceChar, NULL);
		glCompileShader(computeShader);

		// Compilation success check
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
			throw new std::exception("Compute shader compilation failed.");
		}

		// Shader program compilation.
		_shaderProgramId = glCreateProgram();
		glAttachShader(_shaderProgramId, computeShader);
		glLinkProgram(_shaderProgramId);

		// Linking success test
		glGetProgramiv(_shaderProgramId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_shaderProgramId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			throw new std::exception("Linking failed.");
		}

		// The compute shader itself is no longer needed anymore.
		glDeleteShader(computeShader);

		// Make a list of the uniforms in the shader.
		PopulateUniforms();
	}


	ComputeShaderProgram::~ComputeShaderProgram()
	{
	}

}