#pragma once
#include "Renderable.h"

#include<list>

using std::list;

namespace GLEngine
{
	class RenderQueue :
		public Renderable
	{
	public:
		RenderQueue();
		~RenderQueue();

		void AddRenderable(Renderable* newRenderable) { _renderables.push_front(newRenderable); }
		void ClearRenderables() { _renderables.clear(); }

		// Render queue steps.
		virtual void SetGpuState() = 0; // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void Render(SceneManager* sceneManager); // Render all renderables inside the queue, with no gpu state changes in between.
		virtual void ResetGpuDefaultState() = 0; // Post rendering. Resets the GPU to it's original state.


	private:
		list<Renderable*> _renderables;
	};
}

