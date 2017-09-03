#pragma once

#include "Model.h"
#include "EnvironmentMapSky.h"
#include "InstancedModel.h"

using std::map;

namespace GLEngine
{
	class RenderingElements
	{
	public:
		RenderingElements();
		~RenderingElements();

		void PushPointLight(PointLight* pointLight);
		PointLight* PopPointLight();

		EnvironmentMapSky* GetSky() { return _sky; }
		void SetSky(EnvironmentMapSky* sky) { _sky = sky; }

		void PushModel(Model* model);
		InstancedModel* PopInstancedModel();

		void Clear();

	private:
		EnvironmentMapSky* _sky;

		// Stores the resource model pointer as a key, and the list of scene nodes with this model to draw as a key.
		map<Model*, list<SceneNode*>> _instancedModels;

		// Stores the collected point lights.
		list<PointLight*> _pointLights;
	};

}
