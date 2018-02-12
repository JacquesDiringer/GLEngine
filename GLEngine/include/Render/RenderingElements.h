#pragma once

#include <Render\Model.h>
#include <Render\EnvironmentMapSky.h>
#include <Render\InstancedModel.h>

using std::unordered_map;

namespace GLEngine
{
	class RenderingElements
	{
	public:
		RenderingElements();
		~RenderingElements();

		void PushPointLight(PointLight* pointLight);
		PointLight* PopPointLight();

		EnvironmentMapSky* GetSky() const { return _sky; }
		void SetSky(EnvironmentMapSky* sky) { _sky = sky; }

		void PushModel(Model* model);
		InstancedModel* GetNextInstancedModel();
		bool HasMoreInstancedModels() const { return _currentInstancedModel != _instancedModels.end(); };

		void Clear();

	private:
		EnvironmentMapSky* _sky;

		// Stores the resource model pointer as a key, and the list of scene nodes with this model to draw as a key.
		unordered_map<Model*, InstancedModel> _instancedModels;

		// Stores the collected point lights.
		vector<PointLight*> _pointLights;

		unordered_map<Model*, InstancedModel>::iterator _currentInstancedModel;
	};

}
