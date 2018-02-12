#pragma once
#include <Render\Renderable.h>
#include <Render\Model.h>
#include <SceneGraph\SceneNode.h>

#include <vector>

using std::vector;

namespace GLEngine
{
	class InstancedModel :
		public Renderable
	{
	public:
		InstancedModel();
		InstancedModel(Model* resource);
		~InstancedModel();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

		void AddInstance(SceneNode* newNode) { _instancesNodes.push_back(newNode); }

		void SetInstanciationThreshold(int threshold) { _instanciationThreshold = threshold; }

	private:
		Model* _resource;
		vector<SceneNode*> _instancesNodes;
		// Minimum entity number necessary to trigger instanciation.
		int _instanciationThreshold = 100;
	};
}

