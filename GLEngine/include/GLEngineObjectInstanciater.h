#pragma once
#include <Instanciater.h>
#include <SimpleObjectDisplayable.h>
#include <SceneGraph\SceneNode.h>
#include <SceneGraph\SceneManager.h>
#include <Mesh\MeshLoader.h>
#include <Mesh\Mesh.h>
#include <Render\Model.h>
#include <Texture\TextureManager.h>

#include <unordered_map>

using std::unordered_map;

class GLEngineObjectInstanciater
	: public Generator::Instanciater
{
public:
	GLEngineObjectInstanciater(GLEngine::SceneManager* sceneManager, GLEngine::TextureManager* textureManager);
	~GLEngineObjectInstanciater();

	// Add the objects to display to a pending buffer.
	virtual void UpdateDisplayables(const vector<shared_ptr<Generator::Displayable>>& toAdd, const vector<shared_ptr<Generator::Displayable>>& toRemove);

	// Uses the pending buffer to add and remove objects from the scene.
	void Flush(int addCount, int removeCount);

	bool IsFlushCompleted();

private:
	// Adds objects to the GLEngine scene tree.
	bool AddDisplayable(shared_ptr<Generator::Displayable> newDisplayable);
	// Removes objets from the GLEngine scene tree.
	bool RemoveDisplayable(shared_ptr<Generator::Displayable> displayableToRemove);

	// Adds a SimpleObjectDisplayable to the scene tree.
	bool AddSimpleObjectDisplayable(shared_ptr<Generator::SimpleObjectDisplayable> newSimpleObjectDisplayable);
	// Removes a SimpleObjectDisplayable from the scene tree.
	bool RemoveSimpleObjectDisplayable(unordered_map<shared_ptr<Generator::Displayable>, GLEngine::SceneNode*>::iterator findIterator);

	// List of the items that were requested to be added by the generator.
	vector<shared_ptr<Generator::Displayable>> _pendingToAddList;
	// List of the items that were requested to be removed by the generator.
	vector<shared_ptr<Generator::Displayable>> _pendingToRemoveList;

	// A map of the items that are present in the scene tree of the GLEngine.
	unordered_map<shared_ptr<Generator::Displayable>, GLEngine::SceneNode*> _sceneDisplayablesNodes;

	// Map of the already loaded models, to avoid reloading for every model.
	// This feature should be present in the core of the GLEngine.
	unordered_map<string, GLEngine::Model*> _loadedModels;

	// Loads or gets models using the caching system.
	GLEngine::Model* GetModel(const string modelPath, const string diffuseTexturePath);

	// Scene manager.
	GLEngine::SceneManager* _sceneManager;

	// Texture manager.
	GLEngine::TextureManager* _textureManager;

	// OBJ mesh loaded.
	GLEngine::MeshLoader* _meshLoader;
};
