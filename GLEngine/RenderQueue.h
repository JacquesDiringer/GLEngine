#pragma once
#include "Renderable.h"

#include<vector>

using std::vector;

namespace GLEngine
{
	class RenderQueue :
		public Renderable
	{
	public:
		RenderQueue();
		~RenderQueue();

		void AddRenderable(Renderable* newRenderable) { _renderables.push_back(newRenderable); }
		virtual void ClearRenderables() { _renderables.clear(); }

		// Render queue steps.
		virtual void SetGpuState() const = 0; // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const; // Render all renderables inside the queue, with no gpu state changes in between.
		virtual void ResetGpuDefaultState() const = 0; // Post rendering. Resets the GPU to it's original state.


	protected:
		vector<Renderable*> _renderables;
	};
}

