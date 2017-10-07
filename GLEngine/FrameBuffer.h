#pragma once


// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

namespace GLEngine
{
	class FrameBufferManager;

	class FrameBuffer
	{
	public:
		FrameBuffer(int width, int height, FrameBufferManager* manager);
		~FrameBuffer();

		GLuint GetId () const { return _id; }

		void Bind();
		void UnBind();

	protected:
		GLuint _id;
		FrameBufferManager* _manager;
	};

}