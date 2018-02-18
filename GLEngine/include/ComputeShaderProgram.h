#pragma once
#include <ShaderProgram.h>

#include <string>

using std::string;

namespace GLEngine
{
	class ComputeShaderProgram :
		public ShaderProgram
	{
	public:
		ComputeShaderProgram(string computeShaderPath);
		~ComputeShaderProgram();
	};
}