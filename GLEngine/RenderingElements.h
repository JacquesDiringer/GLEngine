#pragma once

#include "Model.h"
#include "EnvironmentMapSky.h"

namespace GLEngine
{
	class RenderingElements
	{
	public:
		RenderingElements();
		~RenderingElements();

		EnvironmentMapSky* GetSky() { return _sky; }
		void SetSky(EnvironmentMapSky* sky) { _sky = sky; }

		void PushModel(Model* model) { _models.push_back(model); }
		Model* PopModel();

	private:
		EnvironmentMapSky* _sky;
		list<Model*> _models;
	};

}
