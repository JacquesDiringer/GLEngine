#include <stdafx.h>
#include <ShaderProgram.h>
#include "Utils.h"

namespace GLEngine
{
	// Reads a vertex shader file and a fragment shader file, and creates the associated a shader program.
	ShaderProgram::ShaderProgram(string vertexShaderFile, string fragmentShaderFile)
	{
		_uniforms = unordered_map<string, Uniform*>();

		// Vertex shader
		GLuint vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		std::string vertexShaderSource = Utils::ReadFile(vertexShaderFile);
		const GLchar* vertexShaderSourceChar = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexShaderSourceChar, NULL);
		glCompileShader(vertexShader);

		// Compilation success check
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			throw new std::exception("Vertex shader compilation failed.");
		}


		//Fragment shader
		GLuint fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string fragmentShaderSource = Utils::ReadFile(fragmentShaderFile);
		const GLchar* fragmentShaderSourceChar = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceChar, NULL);
		glCompileShader(fragmentShader);

		// Compilation success check
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			throw new std::exception("Fragment shader compilation failed.");
		}


		// Shader program
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// Link vertex and fragment shader into one program
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Linking success test
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			throw new std::exception("Linking failed.");
		}

		// Vertex and fragment shaders are not needed anymore after linking.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		_shaderProgramId = shaderProgram;


		// Make a list of the uniforms in the shader.
		GLint count;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)
		const GLsizei bufSize = 100; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &count);

		for (int uniformIndex = 0; uniformIndex < count; uniformIndex++)
		{
			glGetActiveUniform(shaderProgram, (GLuint)uniformIndex, bufSize, &length, &size, &type, name);

			_uniforms.insert(std::pair<string, Uniform*>(string(name), new Uniform(name, (GLuint)uniformIndex, type)));
		}
	}


	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(_shaderProgramId);
	}

	void ShaderProgram::Use()
	{
		glUseProgram(_shaderProgramId);
	}

	Uniform * ShaderProgram::GetUniform(string name)
	{
		 auto iterator = _uniforms.find(name);
		 if (iterator != _uniforms.end())
		 {
			 return iterator->second;
		 }
		 else
		 {
			 throw new std::invalid_argument("No uniform with this name is present or active in the shader.");
		 }
	}
}