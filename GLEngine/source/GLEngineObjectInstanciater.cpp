#include "stdafx.h"
#include "GLEngineObjectInstanciater.h"
#include <Mesh\OBJLoader.h>
#include <Mesh\Mesh.h>

#include <algorithm>
#include <iostream>
#include <utility>

#include <intrin.h>

GLEngineObjectInstanciater::GLEngineObjectInstanciater(GLEngine::SceneManager* sceneManager, GLEngine::TextureManager* textureManager)
	: _sceneManager(sceneManager), _textureManager(textureManager)
{
	_loadedModels = unordered_map<string, GLEngine::Model*>();
	_meshLoader = new GLEngine::OBJLoader();
}


GLEngineObjectInstanciater::~GLEngineObjectInstanciater()
{
	delete _meshLoader;
}

void GLEngineObjectInstanciater::UpdateDisplayables(const vector<shared_ptr<Generator::Displayable>>& toAdd, const vector<shared_ptr<Generator::Displayable>>& toRemove)
{
	int iteratorId = 0;
	for each (shared_ptr<Generator::Displayable> currentDisplayable in toAdd)
	{
		++iteratorId;
		_pendingToAddList.push_back(currentDisplayable);
	}

	iteratorId = 0;
	for each (shared_ptr<Generator::Displayable> currentDisplayable in toRemove)
	{
		++iteratorId;
		_pendingToRemoveList.push_back(currentDisplayable);
	}
}

void GLEngineObjectInstanciater::Flush(int addCount, int removeCount)
{
	vector<shared_ptr<Generator::Displayable>>::iterator displayableIterator;

	// Old elements are removed
	if (!_pendingToRemoveList.empty())
	{
		int toRemoveSize = _pendingToRemoveList.size();
		displayableIterator = _pendingToRemoveList.end();

		for (int removeIndex = 0; removeIndex < removeCount && removeIndex < toRemoveSize; ++removeIndex)
		{
			--displayableIterator;
			RemoveDisplayable(*displayableIterator);
		}

		int numberOfItemsToRemove = std::min(removeCount, (int)_pendingToRemoveList.size());
		for (int removeIndex = 0; removeIndex < numberOfItemsToRemove; ++removeIndex)
		{
			_pendingToRemoveList.pop_back();
		}
	}


	// New elements are added
	if (!_pendingToAddList.empty())
	{
		int toAddSize = _pendingToAddList.size();
		displayableIterator = _pendingToAddList.end();

		for (int addIndex = 0; addIndex < addCount && addIndex < toAddSize; ++addIndex)
		{
			--displayableIterator;
			AddDisplayable(*displayableIterator);
		}

		int numberOfItemsToRemove = std::min(removeCount, (int)_pendingToAddList.size());
		for (int addIndex = 0; addIndex < numberOfItemsToRemove; ++addIndex)
		{
			_pendingToAddList.pop_back();
		}
	}
}

bool GLEngineObjectInstanciater::IsFlushCompleted()
{
	return _pendingToAddList.size() == 0 && _pendingToRemoveList.size() == 0;
}

bool GLEngineObjectInstanciater::AddDisplayable(shared_ptr<Generator::Displayable> newDisplayable)
{
	shared_ptr<Generator::SimpleObjectDisplayable> simpleObject = std::dynamic_pointer_cast<Generator::SimpleObjectDisplayable>(newDisplayable);

	if (simpleObject != nullptr)
	{
		try
		{
			return AddSimpleObjectDisplayable(simpleObject);
		}
		catch (const std::exception&)
		{
			__debugbreak();
		}
	}
	return false;
}

bool GLEngineObjectInstanciater::RemoveDisplayable(shared_ptr<Generator::Displayable> displayableToRemove)
{
	unordered_map<shared_ptr<Generator::Displayable>, GLEngine::SceneNode*>::iterator findIterator = _sceneDisplayablesNodes.find(displayableToRemove);

	if (findIterator == _sceneDisplayablesNodes.end())
	{
		shared_ptr<Generator::SimpleObjectDisplayable> simpleObject = std::dynamic_pointer_cast<Generator::SimpleObjectDisplayable>(displayableToRemove);
		if (simpleObject != nullptr)
		{
			std::cout << "The displayable to remove could not be found, modelName : " << simpleObject->GetModelName() << std::endl;
		}
		else
		{
			std::cout << "The displayable to remove could not be found, modelName : " << std::endl;
		}
		return false;
	}

	return RemoveSimpleObjectDisplayable(findIterator);
}

static int _addCounter = 0;

bool GLEngineObjectInstanciater::AddSimpleObjectDisplayable(shared_ptr<Generator::SimpleObjectDisplayable> newSimpleObjectDisplayable)
{
	++_addCounter;

	string modelPath = newSimpleObjectDisplayable->GetModelName();
	string diffuseTexturePath = newSimpleObjectDisplayable->GetTextureName();

	// Model creation, by cloning a patron Model.
	GLEngine::Model* newModel = new GLEngine::Model(GetModel(modelPath, diffuseTexturePath));

	// Now attach it to a scenenode, so that it becomes present in the scene.
	GLEngine::SceneNode* lNode = _sceneManager->GetRootNode()->CreateChild();

	// Set the position of the new SceneNode.
	GLEngineMath::Matrix4 modelWorldMatrix = GLEngineMath::Matrix4(*reinterpret_cast<GLEngineMath::Matrix4*>(&(newSimpleObjectDisplayable->GetWorldMatrix())));
	/*float* matrixArray = newSimpleObjectDisplayable->GetWorldMatrix();
	GLEngineMath::Matrix4 modelWorldMatrix = GLEngineMath::Matrix4();*/
	lNode->SetRelativeTransformation(modelWorldMatrix);

	// Link the Model to the SceneNode.
	lNode->AddSubElement(newModel);

	// Remember the associated ogre node, to be able to remove it later
	_sceneDisplayablesNodes.insert(std::pair<shared_ptr<Generator::Displayable>, GLEngine::SceneNode*>(newSimpleObjectDisplayable, lNode));

	if (_addCounter % 100 == 0)
	{
		std::cout << "Added " << _addCounter <<" : " << newSimpleObjectDisplayable->GetModelName() << std::endl;
	}

	return true;
}

bool GLEngineObjectInstanciater::RemoveSimpleObjectDisplayable(unordered_map<shared_ptr<Generator::Displayable>, GLEngine::SceneNode*>::iterator findIterator)
{
	GLEngine::SceneNode* nodeToRemove = findIterator->second;

	// TODO: make sure that the node AND the entity are deleted correctly
	nodeToRemove->RemoveFromParentNode();

	// Remove the element from the Displayable SceneNode map
	_sceneDisplayablesNodes.erase(findIterator);

	//std::cout << "Removed : " << ((SimpleObjectDisplayable*)displayableToRemove)->GetTextureName() << std::endl;

	return true;
}

GLEngine::Model * GLEngineObjectInstanciater::GetModel(const string modelPath, const string diffuseTexturePath)
{
	auto findIterator = _loadedModels.find(modelPath);

	if (findIterator != _loadedModels.end())
	{
		return findIterator->second;
	}
	else
	{
		// Load the mesh of the future Model.
		GLEngine::Mesh* loadedMesh = _meshLoader->LoadModel(modelPath);
		loadedMesh->InitializeVao();

		// Load the textures of the future Model.
		GLEngine::Texture2D* diffuseTexture;
		try
		{
			diffuseTexture = _textureManager->GetTexture(diffuseTexturePath);
		}
		catch (const std::exception&)
		{
			__debugbreak();
		}
		GLEngine::Texture2D* specularTexture = _textureManager->GetTexture("C:/Utils/GLEngineMedia/genericSpecular.jpg");
		GLEngine::Texture2D* roughnessTexture = _textureManager->GetTexture("C:/Utils/GLEngineMedia/genericRoughness.jpg");

		// Create the new Model.
		GLEngine::Model* newModel = new GLEngine::Model(loadedMesh, diffuseTexture, specularTexture, roughnessTexture);

		// Cache the new Model, so that it doesn't get loaded again.
		_loadedModels.insert(std::pair<string, GLEngine::Model*>(modelPath, newModel));

		return newModel;
	}
}
