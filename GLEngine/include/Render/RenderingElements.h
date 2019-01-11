#pragma once

#include <Render\Model.h>
#include <Render\EnvironmentMapSky.h>
#include <Render\InstancedModel.h>

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

		void PushModel(Model* model);
		vector<InstancedModel*> GetNextInstancedModels();

		void Clear();

	private:
		// Stores the resource model pointer as a key, and the list of scene nodes with this model to draw as a key.
		map<Model*, InstancedModel> _instancedModels;

		// Stores the collected point lights.
		vector<PointLight*> _pointLights;
	};

}
