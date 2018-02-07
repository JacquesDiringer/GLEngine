#pragma once

#include <Render\Renderable.h>
#include <SceneGraph\SceneElement.h>
#include <Mesh\Mesh.h>
#include <ShaderProgram.h>
#include <Texture\Texture2D.h>
#include <SceneGraph\SceneManager.h>

namespace GLEngine
{
	class EnvironmentMapSky :
		public Renderable, public SceneElement
	{
	public:
		EnvironmentMapSky(Texture2D* environmentMap);
		~EnvironmentMapSky();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

		Texture2D* GetTexture() const { return _environmentMap; }

	private:
		Texture2D* _environmentMap = nullptr;
	};
}