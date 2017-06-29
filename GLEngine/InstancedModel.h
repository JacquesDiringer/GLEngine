#pragma once
#include "Renderable.h"
#include "Model.h"
#include "SceneNode.h"

#include <list>

using std::list;

namespace GLEngine
{
	class InstancedModel :
		public Renderable
	{
	public:
		InstancedModel(Model* resource, list<SceneNode*> instancesNodes);
		~InstancedModel();

		virtual void Render(SceneManager* sceneManager);

	private:
		Model* _resource;
		list<SceneNode*> _instancesNodes;
	};
}

