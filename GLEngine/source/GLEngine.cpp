// GLEngine.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>

#include <thread>
#include <chrono>
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

// Models
#include <Mesh\OBJLoader.h>


// Generator
#include <GLEngineObjectInstanciater.h>
#include <SceneGraphManager.h>
#include <LevelFactory.h>
#include <DependenceTreeDataModel.h>


// Maths
#include <Math\Matrix4.h>
#include <Math\Vector2.h>

#include "..\GLEngine.h"

namespace GLEngine
{
	GLEngineMath::Vector3 _globalCameraPosition = GLEngineMath::Vector3(0, 0, 0);
	GLEngineMath::Vector3 _globalCameraForwardVector = GLEngineMath::Vector3(0, 0, 1);
	GLEngineMath::Vector3 _globalCameraSpeed = GLEngineMath::Vector3(0, 0, 0);
	GLEngineMath::Vector3 _globalTargetPosition = GLEngineMath::Vector3(0, 0, 0);

	float _globalAcceleration = 0.01f;
	float _globalFriction = 0.1f;
	float _sphereRadius = 8.0f;
	float _thetaSpeed = 0, _phiSpeed = 0;
	float _theta = 1.7f, _phi = 0;

	bool _zPressed = false, _sPressed = false, _qPressed = false, _dPressed = false, _majPressed = false;

	GLEngineMath::Vector2 _previousCursorPosition = GLEngineMath::Vector2();
	GLEngineMath::Vector2 _cursorDifference = GLEngineMath::Vector2();

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
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			_majPressed = true;
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
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			_majPressed = false;
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

		// Acceleration multiplier.
		float accelerationMultiplier = 1.0f;
		if (_majPressed)
		{
			accelerationMultiplier = 100.0f;
		}

		// Keyboard handling for first person movements.
		if (_zPressed)
		{
			_globalCameraSpeed = _globalCameraSpeed + _globalCameraForwardVector * _globalAcceleration * accelerationMultiplier;
		}

		if (_sPressed)
		{
			_globalCameraSpeed = _globalCameraSpeed - _globalCameraForwardVector * _globalAcceleration * accelerationMultiplier;
		}

		if (_dPressed)
		{
			_globalCameraSpeed = _globalCameraSpeed - GLEngineMath::Vector3::Cross(GLEngineMath::Vector3(0, 1, 0), _globalCameraForwardVector) * _globalAcceleration * accelerationMultiplier;
		}

		if (_qPressed)
		{
			_globalCameraSpeed = _globalCameraSpeed + GLEngineMath::Vector3::Cross(GLEngineMath::Vector3(0, 1, 0), _globalCameraForwardVector) * _globalAcceleration * accelerationMultiplier;
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

	GLEngineMath::Vector2 UpdateCursorPosition(GLFWwindow * window)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		_cursorDifference = GLEngineMath::Vector2(xpos - _previousCursorPosition.X(), ypos - _previousCursorPosition.Y());

		//std::cout << "new location : x= " << _cursorDifference.X() << "; y= " << _cursorDifference.Y() << std::endl;

		_previousCursorPosition.X(xpos);
		_previousCursorPosition.Y(ypos);

		return _cursorDifference;
	}

	void RefreshGenerator(Generator::SceneGraphManager* sceneManager, Math::Vector3 cameraPos)
	{
		sceneManager->Update(cameraPos, Math::Vector3(0, 0, 0));
	}


	void generatorUpdate(GLEngineObjectInstanciater& instanciater,
		Generator::SceneGraphManager& sceneManager,
		std::thread& proceduralGeneratorThread,
		bool& threadInitialized,
		float& timeSinceLastUpdate,
		const GLEngineMath::Vector3& cameraPosition)
	{
		// Flush refresh
		//if (timeSinceLastUpdate > 0.5 && _ogreInstanciater->IsFlushCompleted())
		{
			Math::Vector3 cameraPos = Math::Vector3(cameraPosition.X(), cameraPosition.Y(), cameraPosition.Z());

			if (threadInitialized)
			{
				proceduralGeneratorThread.join();
			}

			std::chrono::high_resolution_clock::time_point tpSceneManager0 = std::chrono::high_resolution_clock::now();

			sceneManager.Flush();

			/*high_resolution_clock::time_point tpSceneManager1 = high_resolution_clock::now();
			auto sceneManagerDuration = duration_cast<microseconds>(tpSceneManager1 - tpSceneManager0).count();*/
			//std::cout << "Scene manager " << sceneManagerDuration << std::endl;

			proceduralGeneratorThread = std::thread(RefreshGenerator, &sceneManager, cameraPos);

			threadInitialized = true;
			timeSinceLastUpdate = 0;
		}

		//std::chrono::high_resolution_clock::time_point tpOgreInstanciater0 = std::chrono::high_resolution_clock::now();

		instanciater.Flush(30000000, 30000000);

		/*high_resolution_clock::time_point tpOgreInstanciater1 = high_resolution_clock::now();
		auto ogreInstanciaterDuration = duration_cast<microseconds>(tpOgreInstanciater1 - tpOgreInstanciater0).count();
		std::cout << "Instanciater : " << ogreInstanciaterDuration << std::endl;*/
	}

	void InitializeFileReadingTestScene(Generator::SceneGraphManager& sceneManager)
	{
		DataModel::DependenceTreeDataModel dependenceTree = DataModel::DependenceTreeDataModel();
		Generator::LevelFactory* rootFactory = dependenceTree.Read("C:/Utils/GeneratorScenes/demo0_GLEngine/main.txt");
		//Generator::LevelFactory* rootFactory = dependenceTree.Read("C:/Utils/GeneratorScenes/test0/simpleScene.txt");
		//Generator::LevelFactory* rootFactory = dependenceTree.Read("C:/Utils/GeneratorScenes/test0/mainScene.txt");
		//Generator::LevelFactory* rootFactory = dependenceTree.Read("simpleScene.txt");

		//SimpleObjectFactory* testCubeA = new SimpleObjectFactory("A_Brick.mesh", "1d_debug.png", 0, NULL);

		shared_ptr<Generator::SimpleObjectDisplayable> object0 = make_shared<Generator::SimpleObjectDisplayable>("C:/Utils/GLEngineMedia/GeneratorMedia/ABrick.obj", "C:/Utils/OgreMedia/textures/debug_texture.png");
		shared_ptr<Generator::Item> item0 = std::make_shared<Generator::Item>(Math::Matrix4(Math::Vector3(0, 0, 0)), shared_ptr<Generator::Item>(), 100000.0f, object0, rootFactory);
		item0->SetId(10);
		sceneManager.QueueAddItem(item0);
	}

	void GLEngine::InitializeContext()
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
			return;
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
			return;
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

		// Render setting.
		RenderManager* renderManager = new RenderManager(width, height, graphicsResourceManager);

		// Texture manager.
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Camera
		PerspectiveCamera* camera = new PerspectiveCamera(0.1f, 10000.0f, 20.0f, (float)height / (float)width);

		// Post processes.
		//camera->AddPostProcess(new LensPostProcess(width, height, graphicsResourceManager->GetTextureManager()));
		//camera->AddPostProcess(new BloomPostProcess(width, height, graphicsResourceManager->GetTextureManager()));
		camera->AddPostProcess(new GammaCorrectionPostProcess(width, height, graphicsResourceManager->GetTextureManager()));

		// Scene setting

		SceneManager* sceneManager = new SceneManager();
		sceneManager->SetCurrentCamera(camera);

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
		cameraTargetNode->SetRelativeTransformation(GLEngineMath::Matrix4::CreateTranslation(GLEngineMath::Vector3(0, 0, 4)));

		SceneNode* cameraRotatingNode = cameraMainNode->CreateChild();
		cameraRotatingNode->AddSubElement(camera);
		cameraRotatingNode->AddSubElement(new ThirdViewOrientationActor(cameraTargetNode));

		// Sky
		Texture2D* texEnvmapTest = textureManager->GetTexture("C:/Utils/GLEngineMedia/redCliffs.jpg");
		EnvironmentMapSky* envmapTest = new EnvironmentMapSky(texEnvmapTest);

		sceneManager->GetRootNode()->AddSubElement(envmapTest);


		// Environment generator initialization.
		GLEngineObjectInstanciater glEngineInstanciater = GLEngineObjectInstanciater(sceneManager, textureManager);
		Generator::SceneGraphManager generatorSceneManager = Generator::SceneGraphManager(&glEngineInstanciater);
		InitializeFileReadingTestScene(generatorSceneManager);

		// Variable necessary to the generator update.
		std::thread proceduralGeneratorThread;
		bool threadIntialized = false;
		float timeSinceLastUpdate = 0.0f;

		// Game loop
		int frameCount = 0;
		while (!glfwWindowShouldClose(window))
		{
			double timeAtMainLoopStart = glfwGetTime();

			glfwPollEvents();

			UpdateCursorPosition(window);

			// Move the camera "physic" model.
			MoveCamera();

			// Update camera matrix.
			cameraMainNode->SetRelativeTransformation(GLEngineMath::Matrix4::CreateTranslation(_globalCameraPosition));
			cameraTargetNode->SetRelativeTransformation(GLEngineMath::Matrix4::CreateTranslation(_globalCameraForwardVector));

			// Use the environment generator to instanciate new objects.
			generatorUpdate(glEngineInstanciater,
				generatorSceneManager,
				proceduralGeneratorThread,
				threadIntialized,
				timeSinceLastUpdate,
				_globalCameraPosition);

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
	}

	GLEngine::GLEngine::GLEngine()
	{

	}

	GLEngine::GLEngine::~GLEngine()
	{
	}

}