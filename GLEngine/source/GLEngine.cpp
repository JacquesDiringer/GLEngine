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
#include <Math\Vector2.h>

// Maths
#include <Math\Matrix4.h>

// Models
#include <Mesh\OBJLoader.h>


using namespace GLEngine;

Vector3 _globalCameraPosition = Vector3(0, 0, 0);
Vector3 _globalCameraForwardVector = Vector3(0, 0, 1);
Vector3 _globalCameraSpeed = Vector3(0, 0, 0);
Vector3 _globalTargetPosition = Vector3(0, 0, 0);

float _globalAcceleration = 0.01f;
float _globalFriction = 0.1f;
float _sphereRadius = 8.0f;
float _thetaSpeed = 0, _phiSpeed = 0;
float _theta = 1.7f, _phi = 0;

bool _zPressed = false, _sPressed = false, _qPressed = false, _dPressed = false;

Vector2 _previousCursorPosition = Vector2();
Vector2 _cursorDifference = Vector2();

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
	// Mouse rotation for first person.
	_phi += 0.005f * _cursorDifference.X();
	_theta += 0.005f * _cursorDifference.Y();

	_globalCameraForwardVector.X(_sphereRadius * cosf(_theta) * cosf(_phi));
	_globalCameraForwardVector.Y(_sphereRadius * sinf(_theta));
	_globalCameraForwardVector.Z(_sphereRadius * cosf(_theta) * sinf(_phi));

	// Keyboard handling for first person movements.
	if (_zPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + _globalCameraForwardVector * _globalAcceleration;
	}

	if (_sPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed - _globalCameraForwardVector * _globalAcceleration;
	}

	if (_dPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed - Vector3::Cross(Vector3(0, 1, 0), _globalCameraForwardVector) * _globalAcceleration;
	}

	if (_qPressed)
	{
		_globalCameraSpeed = _globalCameraSpeed + Vector3::Cross(Vector3(0, 1, 0), _globalCameraForwardVector) * _globalAcceleration;
	}

	// Add friction.
	_globalCameraSpeed = _globalCameraSpeed * (1 - _globalFriction);

	// Update position accordig to the speed.
	_globalCameraPosition = _globalCameraPosition + _globalCameraSpeed;
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

Vector2 UpdateCursorPosition(GLFWwindow * window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	_cursorDifference = Vector2(xpos - _previousCursorPosition.X(), ypos - _previousCursorPosition.Y());

	//std::cout << "new location : x= " << _cursorDifference.X() << "; y= " << _cursorDifference.Y() << std::endl;

	_previousCursorPosition.X(xpos);
	_previousCursorPosition.Y(ypos);

	return _cursorDifference;
}

int main()
{
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	// Cursor handling.
	// Hide the cursor and keep it inside the window boundaries at all times.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

	glViewport(0, 0, width, height);

	// Graphics Resource Manager.
	GraphicsResourceManager* graphicsResourceManager = new GraphicsResourceManager(width, height);

	// Texture manager.
	TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

	PerspectiveCamera* camera = new PerspectiveCamera(0.1f, 800.0f, 20.0f, (float)height / (float)width);
	
	// Post processes.
	//camera->AddPostProcess(new LensPostProcess(width, height, graphicsResourceManager->GetTextureManager()));
	//camera->AddPostProcess(new BloomPostProcess(width, height, graphicsResourceManager->GetTextureManager()));
	camera->AddPostProcess(new GammaCorrectionPostProcess(width, height, graphicsResourceManager->GetTextureManager()));

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
	Texture2D* planeroughnessTexture = textureManager->GetTexture("C:/Utils/GLEngineMedia/Copper_Plates_Copper_Plates_roughness - Copy.jpg");

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

	SpinnerActor* testSpinner0 = new SpinnerActor(Matrix4::CreateRotationY(-0.5f));
	rotationNode->AddSubElement(testSpinner0);

	SceneNode* extremityNode = rotationNode->CreateChild();
	extremityNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(2, 0.5f, 0)));
	extremityNode->AddSubElement(new Model(suzanneModel));

	SceneNode* arrowsNode = sceneManager->GetRootNode()->CreateChild();
	arrowsNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(1, 2.0f, 0)));
	//arrowsNode->AddSubElement(new Model(arrowsModel));
	//arrowsNode->AddSubElement(new ThirdViewOrientationActor(extremityNode));

	// Camera addition under a scene node.
	// Logic goes like this.
	//									cameraMainNode
	//									/			  \
	//					cameraRotatingNode			cameraTargetNode
	//					/				\
	//		PerspectiveCamera			ThirdViewOrientationActor(cameraTargetNode)

	// The cameraMainNode hold both the target node and the actual camera node.
	// It is the node that will get translated around in the scene.
	SceneNode* cameraMainNode = sceneManager->GetRootNode()->CreateChild();

	SceneNode* cameraTargetNode = cameraMainNode->CreateChild();
	cameraTargetNode->SetRelativeTransformation(Matrix4::CreateTranslation(Vector3(0, 0, 4)));

	SceneNode* cameraRotatingNode = cameraMainNode->CreateChild();
	cameraRotatingNode->AddSubElement(camera);
	cameraRotatingNode->AddSubElement(new ThirdViewOrientationActor(cameraTargetNode));


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

		UpdateCursorPosition(window);

		// Move the camera "physic" model.
		MoveCamera();

		// Update camera matrix.
		cameraMainNode->SetRelativeTransformation(Matrix4::CreateTranslation(_globalCameraPosition));
		cameraTargetNode->SetRelativeTransformation(Matrix4::CreateTranslation(_globalCameraForwardVector));

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

