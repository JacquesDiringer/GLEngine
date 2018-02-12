#include <stdafx.h>
#include <Render\RenderingElements.h>

using std::pair;


namespace GLEngine
{
	RenderingElements::RenderingElements()
	{
		_instancedModels = unordered_map<Model*, InstancedModel>();
		_pointLights = vector<PointLight*>();
		_currentInstancedModel = _instancedModels.begin();
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
		SceneNode* parentNode = model->GetParentNode();

		auto modelIterator = _instancedModels.find(resourceModel);
		if (modelIterator != _instancedModels.end())
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

			// Point at the new beginning in the map.
			_currentInstancedModel = _instancedModels.begin();
		}
	}

	InstancedModel* RenderingElements::GetNextInstancedModel()
	{
		if (_instancedModels.size() > 0 && _currentInstancedModel != _instancedModels.end())
		{
			unordered_map<Model*, InstancedModel>::iterator resultIt = _currentInstancedModel;
			++_currentInstancedModel;

			return &resultIt->second;
		}
	}

	void RenderingElements::Clear()
	{
		_instancedModels.clear();
		_pointLights.clear();
	}
}