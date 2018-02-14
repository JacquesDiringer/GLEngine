// GLEngine.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>

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
#include <ShaderProgram.h>
#include <VertexArrayObject.h>
#include <Texture\TextureManager.h>
#include <Camera\PerspectiveCamera.h>
#include <Render\Model.h>
#include <SceneGraph\SceneManager.h>
#include <SceneGraph\SceneNode.h>
#include <Actor\SpinnerActor.h>
#include <Render\RenderManager.h>
#include <Render\EnvironmentMapSky.h>
#include <Texture\Texture3D.h>
#include <Actor\ThirdViewOrientationActor.h>
#include <Render\PostProcesses\PostProcess.h>
#include <Render\PostProcesses\BloomPostProcess.h>
#include <Render\Lighting\PointLight.h>
#include <Render\PostProcesses\GammaCorrectionPostProcess.h>
#include <Render\PostProcesses\LensPostProcess.h>

// Maths
#include <Math\Matrix4.h>

// Models
#include <Mesh\OBJLoader.h>


using namespace GLEngine;

Vector3 _globalCameraPosition = Vector3(0, 0, 5);
Vector3 _globalCameraSpeed = Vector3(0, 0, 0);
Vector3 _globalTargetPosition = Vector3(0, 0, 0);

float _globalAcceleration = 0.002f;
float _globalFriction = 0.02f;
float _sphereRadius = 8.0f;
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

int main()
{
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFW window creation
	GLFWwindow* window = glfwCreateWindow(1280, 768, "GLEngine test", nullptr, nullptr);
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

	// Graphics Resource Manager.
	GraphicsResourceManager* graphicsResourceManager = new GraphicsResourceManager(width, height);

	// Texture manager.
	TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

	PerspectiveCamera* camera = new PerspectiveCamera(0.1f, 800.0f, 20.0f, (float)height / (float)width);

	// Post processes.
	camera->AddPostProcess(new LensPostProcess(width, height, graphicsResourceManager->GetFrameBufferManager()));
	//camera->AddPostProcess(new BloomPostProcess(width, height, graphicsResourceManager->GetFrameBufferManager()));
	camera->AddPostProcess(new GammaCorrectionPostProcess(width, height, graphicsResourceManager->GetFrameBufferManager()));

	Texture2D* diffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/suzanne_paint.png");

	//Texture2D* specularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/black_white_checker.jpg");
	Texture2D* specularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/suzanne_paint_rougness2_invert.png");

	//Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/black_white_checker.jpg");
	//Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/concreteWallTile.jpg");
	//Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/rougnessPlastic.jpg");
	//Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/suzanne_paint_rougness.png");
	//Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/suzanne_paint_rougness2.png");
	Texture2D* roughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/suzanne_paint_rougness2 - Copie (2).png");

	Texture2D* cylinderdiffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_diffuse.jpg");
	Texture2D* cylinderspecularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_specular.jpg");
	Texture2D* cylinderroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_diffuse.jpg");

	/*Texture2D* planediffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/bucherTile_bucherTile_diffuse.jpg");
	Texture2D* planespecularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/bucherTile_bucherTile_specular.jpg");
	Texture2D* planeroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/bucherTile_bucherTile_roughness.jpg");*/

	// Cool tiles
	/*Texture2D* planediffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Ceramic_mat_Ceramic_test_diffuse.jpg");
	Texture2D* planespecularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_roughness - Copie.jpg");
	Texture2D* planeroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Ceramic_mat_Ceramic_test_roughness.jpg");*/

	Texture2D* planediffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_diffuse.jpg");
	Texture2D* planespecularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_specular.jpg");
	Texture2D* planeroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_roughness.jpg");

	// Cool copper plates
	/*Texture2D* planediffuseTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_diffuse.jpg");
	Texture2D* planespecularTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_specular.jpg");
	Texture2D* planeroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_roughness.jpg");*/

	Texture2D* rgbTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/test_envmap2.png");
	Texture2D* grayTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/rougnessPlastic.jpg");


	// Models testing
	OBJLoader* testLoader = new OBJLoader();

	// Plane resource.
	OBJMesh* planeMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/plane.obj");
	planeMesh->InitializeVao();
	Model* planeModel = new Model(planeMesh, planediffuseTexture, planespecularTexture, planeroughnessTexture);

	// Grudge cylinder resource.
	OBJMesh* grudgeCylinderMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/grudge_cylinder.obj");
	grudgeCylinderMesh->InitializeVao();
	Model* grudgeCylinderModel = new Model(grudgeCylinderMesh, cylinderdiffuseTexture, cylinderspecularTexture, cylinderroughnessTexture);

	// Suzanne resource.
	//OBJMesh* testMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/suzanne_sharp.obj");
	OBJMesh* testMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/suzanne_smooth.obj");
	testMesh->InitializeVao();
	Model* suzanneModel = new Model(testMesh, diffuseTexture, specularTexture, roughnessTexture);

	// Sphere resource.
	OBJMesh* testMesh1 = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/sphere.obj");
	testMesh1->InitializeVao();
	Model* sphereModel1 = new Model(testMesh1, diffuseTexture, specularTexture, roughnessTexture);

	// Arrows resource.
	OBJMesh* arrowsMesh = (OBJMesh*)testLoader->LoadModel("C:/Utils/GLEngineMedia/grudge_cylinder.obj");
	arrowsMesh->InitializeVao();
	Model* cylinderModel = new Model(arrowsMesh, rgbTexture, grayTexture, grayTexture);

	//Texture2D* texEnvmapTest = textureManager->GetTexture("C:/Utils/GLEngineMedia/parking_lot_2k.jpg");
	//Texture2D* texEnvmapTest = textureManager->GetTexture("C:/Utils/GLEngineMedia/pubimage(8).jpg");
	Texture2D* texEnvmapTest = textureManager->GetTexture("C:/Utils/GLEngineMedia/redCliffs.jpg");
	EnvironmentMapSky* envmapTest = new EnvironmentMapSky(texEnvmapTest);

	// Scene setting

	SceneManager* sceneManager = new SceneManager();
	sceneManager->SetCurrentCamera(camera);

	sceneManager->GetRootNode()->AddSubElement(new Model(planeModel));

	SceneNode* skyNode = sceneManager->GetRootNode()->CreateChild();
	skyNode->AddSubElement(envmapTest);

	SceneNode* rotationNode = sceneManager->GetRootNode()->CreateChild();
	rotationNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0.5f, 0)));
	//rotationNode->AddSubElement(new Model(testModel1));

	SpinnerActor* testSpinner0 = new SpinnerActor(&Matrix4::CreateRotationY(-0.5f));
	rotationNode->AddSubElement(testSpinner0);

	SceneNode* extremityNode = rotationNode->CreateChild();
	extremityNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(2, 0.5f, 0)));
	extremityNode->AddSubElement(new Model(suzanneModel));

	SceneNode* arrowsNode = sceneManager->GetRootNode()->CreateChild();
	arrowsNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(1, 2.0f, 0)));
	//arrowsNode->AddSubElement(new Model(arrowsModel));
	//arrowsNode->AddSubElement(new ThirdViewOrientationActor(extremityNode));

	// Camera addition under a scene node.
	//SceneNode* cameraNode = sceneManager->GetRootNode()->CreateChild();
	SceneNode* cameraNode = sceneManager->GetRootNode()->CreateChild();
	//SceneNode* cameraNode = extremityNode->CreateChild();
	cameraNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0, 4)));
	cameraNode->AddSubElement(camera);
	//cameraNode->AddSubElement(new ThirdViewOrientationActor(extremityNode));
	cameraNode->AddSubElement(new ThirdViewOrientationActor(sceneManager->GetRootNode()));

	/*extremityNode->AddSubElement(camera);
	extremityNode->AddSubElement(new ThirdViewOrientationActor(rotationNode));*/


	// Point light.
	PointLight* testPointLight = new PointLight(Vector3(20, 20, 20), 10);
	PointLight* testPointLight1 = new PointLight(Vector3(20, 20, 20), 10);
	PointLight* testPointLight2 = new PointLight(Vector3(20, 20, 20), 10);

	// Point light node.
	//SceneNode* pointLightNode = sceneManager->GetRootNode()->CreateChild();
	SceneNode* pointLightNode = extremityNode->CreateChild();
	pointLightNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0.5f, 3)));
	pointLightNode->AddSubElement(testPointLight);

	SceneNode* pointLightNode1 = extremityNode->CreateChild();
	pointLightNode1->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0.5f, 4)));
	pointLightNode1->AddSubElement(testPointLight1);

	SceneNode* pointLightNode2 = extremityNode->CreateChild();
	pointLightNode2->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0.5f, 5)));
	pointLightNode2->AddSubElement(testPointLight2);

	SceneNode* nodeToDelete;

	// Instancing tests, array.
	/*for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			SceneNode* currentNode = sceneManager->GetRootNode()->CreateChild();
			currentNode->SetRelativeTransformation(Matrix4::CreateTranslation(new Vector3(i * 3, 0, j * 3)));
			currentNode->AddSubElement(new Model(grudgeCylinderModel));
			if (i == 2 && j == 0)
			{
				nodeToDelete = currentNode;
			}
		}
	}

	nodeToDelete->RemoveFromParentNode();*/

	// Render setting.
	RenderManager* renderManager = new RenderManager(width, height, graphicsResourceManager);

	// Game loop
	int frameCount = 0;
	while (!glfwWindowShouldClose(window))
	{
		/*if (frameCount % 2 == 0)
		{
			nodeToDelete = sceneManager->GetRootNode()->CreateChild();
			nodeToDelete->AddSubElement(new Model(grudgeCylinderModel));
		}
		else
		{
			nodeToDelete->RemoveFromParentNode();
			delete(nodeToDelete);
		}*/

		double timeAtMainLoopStart = glfwGetTime();

		glfwPollEvents();

		// Move the camera "physic" model.
		MoveCamera();

		// Update camera matrix.
		cameraNode->SetRelativeTransformation(Matrix4::CreateTranslation(_globalCameraPosition));
		//camera->SetPositionAndTarget(_globalCameraPosition, _globalTargetPosition);

		//Scene graph update.
		sceneManager->Update();

		double timeAfterUpdate = glfwGetTime();

		// Rendering.
		renderManager->Render(sceneManager, graphicsResourceManager);

		// Main buffers swap.
		glfwSwapBuffers(window);

		double timeAtMainLoopEnd = glfwGetTime();

		double updateTime = timeAfterUpdate - timeAtMainLoopStart;
		double renderTime = timeAtMainLoopEnd - timeAfterUpdate;
		double mainLoopTime = timeAtMainLoopEnd - timeAtMainLoopStart;

		if (frameCount % 10 == 0)
		{
			std::cout << "Update time: " << updateTime * 1000 << "ms; FPS: " << 1 / updateTime << std::endl;
			std::cout << "Render time: " << renderTime * 1000 << "ms; FPS: " << 1 / renderTime << std::endl;
			std::cout << "Main loop time: " << mainLoopTime * 1000 << "ms; FPS: " << 1 / mainLoopTime << std::endl << std::endl;
		}
		++frameCount;
	}

	// Clean resources
	glfwTerminate();

	return 0;
}

