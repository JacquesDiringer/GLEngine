#pragma once

#include <Texture\Texture2D.h>
#include <Render\Renderable.h>

namespace GLEngine
{
	class Light
		: public Renderable
	{
	public:
		Light();
		virtual ~Light();

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const = 0;

		void SetGeometryTexture(Texture2D* geometryTexture) { _geometryTexture = geometryTexture; }
		void SetDiffuseTexture(Texture2D* diffuseTexture) { _diffuseTexture = diffuseTexture; }
		void SetSpecularRoughnessTexture(Texture2D* specularRoughnessTexture) { _specularRoughnessTexture = specularRoughnessTexture; }

	protected:
		// G-Buffer textures (excepting the emissive, useless here), to be set before calling Render.
		Texture2D *_geometryTexture, *_diffuseTexture, *_specularRoughnessTexture;
	};

}
