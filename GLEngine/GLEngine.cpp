// GLEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <math.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Engine classes
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "TextureManager.h"
#include "PerspectiveCamera.h"

// Maths
#include "Matrix4.h"

// Models
#include "OBJLoader.h"


using namespace GLEngine;

Vector3 _globalCameraPosition = Vector3(0, 0, 0);
Vector3 _globalCameraSpeed = Vector3(0, 0, 0);
Vector3 _globalTargetPosition = Vector3(0, 0, 0);

float _globalAcceleration = 0.01f;
float _globalFriction = 0.1f;
float _sphereRadius = 5.0f;
float _thetaSpeed = 0, _phiSpeed = 0;
float _theta = 0, _phi = 0;

bool _zPressed = false, _sPressed = false, _qPressed = false, _dPressed = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		_zPressed = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		_sPressed = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		_dPressed = true;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		_qPressed = true;
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		_zPressed = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		_sPressed = false;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		_dPressed = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		_qPressed = false;
	}
}

// Returns the VAO id
VertexArrayObject* InitBuffers()
{
	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	 // Bottom Left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f	 // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	list<int> elements = list<int>();
	elements.push_back(0);
	elements.push_back(1);
	elements.push_back(3);

	elements.push_back(1);
	elements.push_back(2);
	elements.push_back(3);

	list<Vector3*> positions = list<Vector3*>();
	positions.push_back(new Vector3(0.5f, 0.5f, -1.0f));
	positions.push_back(new Vector3(0.5f, -0.5f, -1.0f));
	positions.push_back(new Vector3(-0.5f, -0.5f, -1.0f));
	positions.push_back(new Vector3(-0.5f, 0.5f, -1.0f));

	list<Vector3*> normals = list<Vector3*>();
	normals.push_back(new Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(new Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(new Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(new Vector3(1.0f, 1.0f, 0.0f));

	list<Vector2*> uVs = list<Vector2*>();
	uVs.push_back(new Vector2(0.0f, 0.0f));
	uVs.push_back(new Vector2(0.0f, 1.0f));
	uVs.push_back(new Vector2(1.0f, 1.0f));
	uVs.push_back(new Vector2(1.0f, 0.0f));

	//VertexArrayObject* result = new VertexArrayObject(elements, positions, normals);
	VertexArrayObject* result = new VertexArrayObject(elements, positions, uVs);

	return result;
}

void Render(VertexArrayObject* vao, ShaderProgram* shaderProgram, TextureManager* texManager, PerspectiveCamera* camera)
{
	// Tests
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shaderProgram->Use();

	GLfloat timeValue = glfwGetTime();
	//shaderProgram->GetUniform("time")->SetValue(timeValue);

	// Textures
	Texture2D* tex0 = texManager->GetTexture("phone4.jpg");
	Texture2D* tex1 = texManager->GetTexture("strike_vector_background.jpg");

	texManager->AssignTextureToUnit(tex0);
	texManager->AssignTextureToUnit(tex1);

	//shaderProgram->GetUniform("testImage0")->SetValue((GLuint)tex0->GetBoundUnit());
	//shaderProgram->GetUniform("testImage1")->SetValue((GLuint)tex1->GetBoundUnit());

	// Transforms
	/*Matrix4 projectionMatrix = Matrix4::CreateSymetricProjectionFrustum(0.1f, 100.0f, 0.05, 0.05);
	Matrix4 viewMatrix = Matrix4::CreateTargetPositionCameraYAxis(Vector3(sin(timeValue),cos(timeValue),0), Vector3(0,0,-1));
	Matrix4 transformMatrix = projectionMatrix * viewMatrix;

	shaderProgram->GetUniform("transform")->SetValue(&transformMatrix);*/
	Matrix4 viewProjectionMatrix = camera->GetViewProjection();
	shaderProgram->GetUniform("transform")->SetValue(&viewProjectionMatrix);

	vao->Bind();
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, vao->GetElementsCount(), GL_UNSIGNED_INT, 0);
	vao->UnBind();

	texManager->FreeUnits();
}

void MoveCamera()
{
	if (_zPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + Vector3(0, 0, -_globalAcceleration);
		_thetaSpeed += _globalAcceleration;
	}

	if (_sPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + Vector3(0, 0, _globalAcceleration);
		_thetaSpeed -= _globalAcceleration;
	}

	if (_dPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + Vector3(_globalAcceleration, 0, 0);
		_phiSpeed -= _globalAcceleration;
	}

	if (_qPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + Vector3(-_globalAcceleration, 0, 0);
		_phiSpeed += _globalAcceleration;
	}

	//_globalCameraPosition = _globalCameraPosition + _globalCameraSpeed;
	//_globalTargetPosition = _globalTargetPosition + Vector3(0, 0, -1);

	_globalCameraSpeed = _globalCameraSpeed * (1 - _globalFriction);
	_globalCameraSpeed = _globalCameraSpeed * (1 - _globalFriction);


	// 3rd view camera.
	_theta += _thetaSpeed;
	_phi += _phiSpeed;

	_thetaSpeed = _thetaSpeed * (1 - _globalFriction);
	_phiSpeed = _phiSpeed * (1 - _globalFriction);

	_globalCameraPosition.X(_sphereRadius * cosf(_theta) * cosf(_phi));
	_globalCameraPosition.Y(_sphereRadius * sinf(_theta));
	_globalCameraPosition.Z(_sphereRadius * cosf(_theta) * sinf(_phi));
}

int main()
{
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFW window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLEngine test", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Key callback function
	glfwSetKeyCallback(window, key_callback);


	// GLEW intialization
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	// Viewport setting
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);


	// Vertex buffer initialization.
	//VertexArrayObject* vao = InitBuffers();

	// Texture manager.
	TextureManager* textureManager = new TextureManager();
	/*Texture2D* testTexture = textureManager->GetTexture("phone4.jpg");
	Texture2D* testTexture2 = textureManager->GetTexture("strike_vector_background.jpg");*/
	
	// Shaders initialization.
	ShaderProgram* shaderProgram = new ShaderProgram("VertexShader.txt", "FragmentShader.txt", textureManager);

	PerspectiveCamera* camera = new PerspectiveCamera(0.1f, 100.0f, 20.0f, (float)height / (float)width);


	// Models testing
	OBJLoader* testLoader = new OBJLoader();
	//OBJMesh* testModel = (OBJMesh*)testLoader->LoadModel("C:/Users/Jacques/Documents/GLEngineMedia/suzanne_sharp.obj");
	OBJMesh* testModel = (OBJMesh*)testLoader->LoadModel("C:/Users/Jacques/Documents/GLEngineMedia/suzanne_smooth.obj");
	//OBJMesh* testModel = (OBJMesh*)testLoader->LoadModel("C:/Users/Jacques/Documents/GLEngineMedia/flyingCar.obj");
	//OBJMesh* testModel = (OBJMesh*)testLoader->LoadModel("C:/Users/Jacques/Documents/GLEngineMedia/cube.obj");
	//OBJMesh* testModel = (OBJMesh*)testLoader->LoadModel("C:/Users/Jacques/Documents/GLEngineMedia/sphere.obj");

	VertexArrayObject* vao = new VertexArrayObject(testModel->GetElementsList(), testModel->GetPositionsList(), testModel->GetNormalsList());

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Move the camera "physic" model.
		MoveCamera();

		// Update camera matrix.
		camera->SetPositionAndTarget(_globalCameraPosition, _globalTargetPosition);

		// Rendering.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render(vao, shaderProgram, textureManager, camera);

		glfwSwapBuffers(window);
	}

	// Clean resources
	glfwTerminate();

    return 0;
}

