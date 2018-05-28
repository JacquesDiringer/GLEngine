#pragma once

#include <Render\Renderable.h>
#include <SceneGraph\SceneElement.h>
#include <Mesh\Mesh.h>
#include <ShaderProgram.h>
#include <Texture\Texture2D.h>
#include <SceneGraph\SceneManager.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API EnvironmentMapSky :
		public Renderable
	{
	public:
		EnvironmentMapSky(Texture2D* environmentMap);
		~EnvironmentMapSky();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

		Texture2D* GetTexture() const { return _environmentMap; }

	private:
		Texture2D* _environmentMap = nullptr;
	};
}