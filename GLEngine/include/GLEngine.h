#pragma once

// GLEW
#define GLEW_STATIC
#include<GL\glew.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API GLEngine
	{
	public:
		GLEngine();
		~GLEngine();

		void InitializeContext();

	private:
	};
}