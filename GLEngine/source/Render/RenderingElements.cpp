#include <stdafx.h>
#include <Render\RenderingElements.h>

using std::pair;


namespace GLEngine
{
	RenderingElements::RenderingElements()
	{
		_instancedModels = unordered_map<Model*, vector<SceneNode*>*>();
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
		SceneNode* parentNode = model->GetParentNode();

		auto modelIterator = _instancedModels.find(resourceModel);
		if (modelIterator != _instancedModels.end())
		{
			modelIterator->second->push_back(parentNode);
		}
		else
		{
			vector<SceneNode*>* newModelVector = new vector<SceneNode*>();
			newModelVector->push_back(parentNode);
			_instancedModels[resourceModel] = newModelVector;
		}
	}

	InstancedModel* RenderingElements::PopInstancedModel()
	{
		if (_instancedModels.size() > 0)
		{
			// Find the first element of the map, use it to instanciate our result value, then delete it from the map.
			auto frontPair = _instancedModels.begin();
			InstancedModel* result = new InstancedModel((*frontPair).first, (*frontPair).second);
			_instancedModels.erase(frontPair);

			return result;
		}

		return nullptr;
	}

	void RenderingElements::Clear()
	{
		_instancedModels.clear();
		_pointLights.clear();
	}
}