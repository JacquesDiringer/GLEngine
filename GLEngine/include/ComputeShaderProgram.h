#pragma once
#include <ShaderProgram.h>
#include <GraphicsDeviceManager.h>

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

		// Launch compute work groups.
		void UseAndDispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, const GraphicsDeviceManager& graphicsDeviceManager);

	private:
		GLuint _group_size_x, _group_size_y, _group_size_z;
	};
}