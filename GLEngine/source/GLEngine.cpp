// GLEngine.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

// Engine classes
#include <Texture\TextureManager.h>
#include <Camera\PerspectiveCamera.h>
#include <Render\Model.h>
#include <SceneGraph\SceneManager.h>
#include <SceneGraph\SceneNode.h>
#include <Actor\SpinnerActor.h>
#include <Render\RenderManager.h>
#include <Render\EnvironmentMapSky.h>
#include <Actor\ThirdViewOrientationActor.h>
#include <Render\PostProcesses\PostProcess.h>
#include <Render\PostProcesses\BloomPostProcess.h>
#include <Render\Lighting\PointLight.h>
#include <Render\PostProcesses\GammaCorrectionPostProcess.h>
#include <Render\PostProcesses\LensPostProcess.h>

// Models
#include <Mesh\OBJLoader.h>

// Maths
#include <Math\Matrix4.h>
#include <Math\Vector2.h>

#include "..\GLEngine.h"

namespace GLEngine
{
	bool * _globalKeys;

	GLFWwindow * _window = nullptr;

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// When a user presses the escape key, we set the WindowShouldClose property to true, 
		// closing the application
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// 0 = z;
		// 1 = s;
		// 2 = d;
		// 3 = q;
		// 4 = maj;

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			_globalKeys[0] = true;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			_globalKeys[1] = true;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			_globalKeys[2] = true;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			_globalKeys[3] = true;
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			_globalKeys[4] = true;
		}

		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			_globalKeys[0] = false;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			_globalKeys[1] = false;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			_globalKeys[2] = false;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			_globalKeys[3] = false;
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			_globalKeys[4] = false;
		}
	}

	void GLErrorCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	void GLEngine::InitializeContext(int width, int height, bool* keys)
	{
		// Set the pointer to the keys tab to cheat the callbacks.
		// TODO: implement a cleaner system.
		_globalKeys = keys;

		// GLFW initialization
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// GLFW window creation
		_window = glfwCreateWindow(width, height, "GLEngine test", nullptr, nullptr);
		if (_window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);

		// Key callback function
		glfwSetKeyCallback(_window, key_callback);

		// Cursor handling.
		// Hide the cursor and keep it inside the window boundaries at all times.
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		// GLEW intialization
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}

		// Viewport setting
		int actualWidth, actualHeight;
		glfwGetFramebufferSize(_window, &actualWidth, &actualHeight);

		// During init, enable debug output
		/*glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)GLErrorCallback, 0);

		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);*/

		glViewport(0, 0, actualWidth, actualHeight);
	}

	int GLEngine::GLFWWindowShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	double GLEngine::GLFWGetTime()
	{
		return glfwGetTime();
	}

	void GLEngine::GLFWPollEvents()
	{
		glfwPollEvents();
	}

	void GLEngine::GLFWSwapBuffers()
	{
		glfwSwapBuffers(_window);
	}

	void GLEngine::GLFWTerminate()
	{
		glfwTerminate();
	}

	void GLEngine::GLFWGetCursorPos(double * xPos, double * yPos)
	{
		glfwGetCursorPos(_window, xPos, yPos);
	}

	GLEngine::GLEngine::GLEngine()
	{
		// Void.
	}

	GLEngine::GLEngine::~GLEngine()
	{
		// Void.
	}

}