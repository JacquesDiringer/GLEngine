#include <stdafx.h>
#include <Render\RenderingElements.h>
#include <iostream>

using std::pair;


namespace GLEngine
{
	RenderingElements::RenderingElements()
	{
		_instancedModels = unordered_map<Model*, InstancedModel>();
		_pointLights = vector<PointLight*>();
	}

	RenderingElements::~RenderingElements()
	{
	}

	void RenderingElements::PushPointLight(PointLight * pointLight)
	{
		_pointLights.push_back(pointLight);
	}

	PointLight * RenderingElements::PopPointLight()
	{
		if (_pointLights.size() > 0)
		{
			PointLight* result = _pointLights.back();
			_pointLights.pop_back();

			return result;
		}

		return nullptr;
	}

	void RenderingElements::PushModel(Model * model)
	{
		// Instanced rendering vector.
		Model* resourceModel = model->GetResource();
		SceneNode* parentNode = dynamic_cast<SceneNode*>(model->GetParent());

		auto modelIterator = _instancedModels.find(resourceModel);
		if (modelIterator != _instancedModels.end() && parentNode != nullptr)
		{
			// If the Model already has it's instanced version, just add this instance.
			modelIterator->second.AddInstance(parentNode);
		}
		else
		{
			// Add the new Model as a new InstancedModel in the map.
			InstancedModel newModelVector = InstancedModel(resourceModel);
			newModelVector.AddInstance(parentNode);
			_instancedModels[resourceModel] = newModelVector;
		}
	}

	vector<InstancedModel*> RenderingElements::GetNextInstancedModels()
	{
		vector<InstancedModel*> result = vector<InstancedModel*>();

		for (unordered_map<Model*, InstancedModel>::iterator mapIt = _instancedModels.begin(); mapIt != _instancedModels.end(); mapIt++)
		{
			result.push_back(&mapIt->second);
		}

		return result;
	}

	void RenderingElements::Clear()
	{
		_instancedModels.clear();
		_pointLights.clear();
	}
}