#include "stdafx.h"
#include "InstancedModel.h"


namespace GLEngine
{
	InstancedModel::InstancedModel(Model* resource, list<SceneNode*> instancesNodes)
		: Renderable(), _resource(resource), _instancesNodes(instancesNodes)
	{
	}


	InstancedModel::~InstancedModel()
	{
		_instancesNodes.clear();
	}

	void InstancedModel::Render(SceneManager * sceneManager)
	{
		for each (SceneNode* currentNode in _instancesNodes)
		{
			_resource->RenderResource(sceneManager, currentNode);
		}
	}
}