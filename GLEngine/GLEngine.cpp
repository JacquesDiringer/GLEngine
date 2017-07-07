// GLEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

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
#include "Model.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "SpinnerActor.h"
#include "RenderManager.h"
#include "EnvironmentMapSky.h"

// Maths
#include "Matrix4.h"

// Models
#include "OBJLoader.h"


using namespace GLEngine;

Vector3 _globalCameraPosition = Vector3(0, 0, 5);
Vector3 _globalCameraSpeed = Vector3(0, 0, 0);
Vector3 _globalTargetPosition = Vector3(0, 1, 0);

float _globalAcceleration = 0.01f;
float _globalFriction = 0.1f;
float _sphereRadius = 3.0f;
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

	_globalCameraSpeed = _globalCameraSpeed * (1 - _globalFriction);
	_globalCameraSpeed = _globalCameraSpeed * (1 - _globalFriction);

	// Arrow translation
	/*_globalCameraPosition.X(_globalCameraPosition.X() + _globalCameraSpeed.X());
	_globalCameraPosition.Y(_globalCameraPosition.Y() + _globalCameraSpeed.Y());
	_globalCameraPosition.Z(_globalCameraPosition.Z() + _globalCameraSpeed.Z());*/


	// 3rd view camera.
	_theta += _thetaSpeed;
	_phi += _phiSpeed;

	_thetaSpeed = _thetaSpeed * (1 - _globalFriction);
	_phiSpeed = _phiSpeed * (1 - _globalFriction);

	_globalCameraPosition.X(_sphereRadius * cosf(_theta) * cosf(_phi));
	_globalCameraPosition.Y(_sphereRadius * sinf(_theta));
	_globalCameraPosition.Z(_sphereRadius * cosf(_theta) * sinf(_phi));


	// Tests
	//_globalTargetPosition.X(cos(glfwGetTime() * 5.0));
}

Texture2D* ConvoluteEnvironmentMap(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager, float width, float height)
{
	int convolutedWidth = 512;
	int convolutedHeight = 256;

	// Image processing frame buffer.
	GLuint imageProcessFrameBuffer;
	glGenFramebuffers(1, &imageProcessFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, imageProcessFrameBuffer);

	glViewport(0, 0, convolutedWidth, convolutedHeight);

	// This texture will hold the convoluted environment map.
	GLuint convolutedEnvmap;
	glGenTextures(1, &convolutedEnvmap);
	glBindTexture(GL_TEXTURE_2D, convolutedEnvmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, convolutedWidth, convolutedHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Link the texture to the framebuffer color attachment 0.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, convolutedEnvmap, 0);

	// Enable drawing in the lighting texture.
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Check if the frame buffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw new std::exception("Convolution frame buffer incomplete.");
	}

	// Activate the convolution shader.
	ShaderProgram* convolutionShader = graphicsResourceManager->GetEnvmapConvolutionShader();
	convolutionShader->Use();

	// Set the blending mode to accumulate each new render.
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Set the environment map texture.
	graphicsResourceManager->GetTextureManager()->AssignTextureToUnit(environmentMap);
	convolutionShader->GetUniform("envmap")->SetValue((GLuint)environmentMap->GetBoundUnit());

	// Fetch vector in the envmap.
	Vector3* currentFetchVector = new Vector3();

	int samplesNumber = 5000;

	// Bind the screen VAO.
	VertexArrayObject* screenVAO = graphicsResourceManager->GetScreenVAO();
	screenVAO->Bind();
	{
		//for (float integrationAngle = (float)M_PI_2 * 0.05f; integrationAngle > 0; integrationAngle--)
		{
			float integrationAngle = (float)M_PI_2 * 0.1f;

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			convolutionShader->GetUniform("integrationAngle")->SetValue(integrationAngle);
			//convolutionShader->GetUniform("integrationAngle")->SetValue(integrationAngle);
			float integrationSolidAngleOverSphereSolidAngle = (float)(0.5 * (1 - cos(integrationAngle)));
			convolutionShader->GetUniform("divisor")->SetValue(3/((float)samplesNumber * integrationSolidAngleOverSphereSolidAngle));

			for (int fetchIndex = 0; fetchIndex < samplesNumber; fetchIndex++)
			{
				// Randomly set the values of the fetch vector, but don't forget to normalize it.
				currentFetchVector->X(rand() % 10000 - 5000);
				currentFetchVector->Y(rand() % 10000 - 5000);
				currentFetchVector->Z(rand() % 10000 - 5000);
				currentFetchVector->Normalize();

				convolutionShader->GetUniform("currentFetchVector")->SetValue(currentFetchVector);

				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}

			//glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	screenVAO->UnBind();

	// Disable blending.
	glDisable(GL_BLEND);

	// Unbind and delete frame buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &imageProcessFrameBuffer);

	glViewport(0, 0, width, height);

	// Free texture units from the environment map.
	graphicsResourceManager->GetTextureManager()->FreeUnits();

	//// Test giving this texture to a the combine shader
	//ShaderProgram* combiner = graphicsResourceManager->GetPbrCombinerShader();
	//combiner->Use();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, convolutedEnvmap);
	//combiner->GetUniform("emissiveGTexture")->SetValue(0);

	//glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//screenVAO->Bind();
	//glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
	//screenVAO->UnBind();

	//graphicsResourceManager->GetTextureManager()->FreeUnits();

	return new Texture2D(convolutedEnvmap, environmentMap->GetWidth(), environmentMap->GetHeight());
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

	// Texture manager.
	TextureManager* textureManager = new TextureManager();

	PerspectiveCamera* camera = new PerspectiveCamera(0.1f, 800.0f, 20.0f, (float)height / (float)width);


	// Models testing
	OBJLoader* testLoader = new OBJLoader();

	// Suzanne resource.
	//OBJMesh* testMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/suzanne_sharp.obj");
	OBJMesh* testMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/suzanne_smooth.obj");
	testMesh->InitializeVao();
	Model* testModel = new Model(testMesh);

	// Sphere resource.
	OBJMesh* testMesh1 = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/sphere.obj");
	testMesh1->InitializeVao();
	Model* testModel1 = new Model(testMesh1);

	// Envmap.
	OBJMesh* sphereMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/sphere_UVs.obj");
	sphereMesh->InitializeVao();

	Texture2D* texEnvmapTest = textureManager->GetTexture("C:/Utils/GLEngineMedia/parking_lot_2k.jpg");
	EnvironmentMapSky* envmapTest = new EnvironmentMapSky(sphereMesh, texEnvmapTest);

	// Scene setting

	SceneManager* sceneManager = new SceneManager();
	sceneManager->SetCurrentCamera(camera);

	SceneNode* skyNode = sceneManager->GetRootNode()->CreateChild();
	skyNode->AddSubElement(envmapTest);

	SceneNode* rotationNode = sceneManager->GetRootNode()->CreateChild();
	//rotationNode->AddSubElement(new Model(testModel1));

	SpinnerActor* testSpinner0 = new SpinnerActor(&Matrix4::CreateRotationY(-0.5f));
	//rotationNode->AddSubElement(testSpinner0);

	SceneNode* extremityNode = rotationNode->CreateChild();
	extremityNode->SetRelativeTransformation(&Matrix4::CreateTranslation(Vector3(2, 0, 0)));
	extremityNode->AddSubElement(new Model(testModel));

	SpinnerActor* testSpinner = new SpinnerActor(&Matrix4::CreateRotationY(1.0f));
	//extremityNode->AddSubElement(testSpinner);

	// Instancing tests, array.
	for (int i = -2; i < 2; i++)
	{
		for (int j = -2; j < 2; j++)
		{
			SceneNode* currentNode = sceneManager->GetRootNode()->CreateChild();
			currentNode->SetRelativeTransformation(Matrix4::CreateTranslation(new Vector3(i*3, 0, j*3)));
			currentNode->AddSubElement(new Model(testModel));
		}
	}

	// Render setting.
	RenderManager* renderManager = new RenderManager(width, height);

	// Graphics Resource Manager.
	GraphicsResourceManager* graphicsResourceManager = new GraphicsResourceManager(textureManager);

	// Image based lighting.
	Texture2D* iblTexture = ConvoluteEnvironmentMap(texEnvmapTest, graphicsResourceManager, width, height);
	renderManager->SetIblTexture(iblTexture);

	// Game loop
	int frameCount = 0;
	while (!glfwWindowShouldClose(window))
	{
		double timeAtMainLoopStart = glfwGetTime();

		glfwPollEvents();

		// Move the camera "physic" model.
		MoveCamera();

		// Update camera matrix.
		camera->SetPositionAndTarget(_globalCameraPosition, _globalTargetPosition);

		//Scene graph update.
		sceneManager->Update();

		// Rendering.
		renderManager->Render(sceneManager, graphicsResourceManager);

		// Main buffers swap.
		glfwSwapBuffers(window);

		double timeAtMainLoopEnd = glfwGetTime();

		double mainLoopTime = timeAtMainLoopEnd - timeAtMainLoopStart;

		if (frameCount % 10 == 0)
		{
			std::cout << "Main loop time: " << mainLoopTime * 1000 << "ms; FPS: " << 1 / mainLoopTime << std::endl;
		}
		++frameCount;
	}

	// Clean resources
	glfwTerminate();

    return 0;
}

