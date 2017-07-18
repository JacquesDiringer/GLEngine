#pragma once


// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

namespace GLEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();

		void Bind();
		void UnBind();

	protected:
		GLuint _id;
	};

}