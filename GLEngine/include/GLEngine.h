#pragma once

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

		void InitializeContext(int width, int height, bool* keys);

		// GLFW functions.
		int GLFWWindowShouldClose();
		double GLFWGetTime();
		void GLFWPollEvents();
		void GLFWSwapBuffers();
		void GLFWTerminate();
		void GLFWGetCursorPos(double * xPos, double * yPos);

	private:
	};
}