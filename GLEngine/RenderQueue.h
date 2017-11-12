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
		virtual void ClearRenderables() { _renderables.clear(); }

		// Render queue steps.
		virtual void SetGpuState() const = 0; // Prior to rendering. Sets the Gpu to the desired state for rendering.
		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const; // Render all renderables inside the queue, with no gpu state changes in between.
		virtual void ResetGpuDefaultState() const = 0; // Post rendering. Resets the GPU to it's original state.


	protected:
		list<Renderable*> _renderables;
	};
}

