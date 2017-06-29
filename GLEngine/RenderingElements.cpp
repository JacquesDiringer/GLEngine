#include "stdafx.h"
#include "RenderingElements.h"

using std::pair;


namespace GLEngine
{
	RenderingElements::RenderingElements()
	{
		_instancedModels = map<Model*, list<SceneNode*>>();
	}

	RenderingElements::~RenderingElements()
	{
	}

	void RenderingElements::PushModel(Model * model)
	{
		// Instanced rendering list.
		Model* resourceModel = model->GetResource();
		SceneNode* parentNode = model->GetParentNode();

		_instancedModels[resourceModel].push_back(parentNode);
	}

	InstancedModel* RenderingElements::PopInstancedModel()
	{
		if (_instancedModels.size() > 0)
		{
			// Find the first element of the map, use it to instanciate our result value, then delete it from the map.
			map<Model*, list<SceneNode*>>::iterator frontPair = _instancedModels.begin();
			InstancedModel* result = new InstancedModel((*frontPair).first, (*frontPair).second);
			_instancedModels.erase(frontPair);

			return result;
		}

		return nullptr;
	}
}