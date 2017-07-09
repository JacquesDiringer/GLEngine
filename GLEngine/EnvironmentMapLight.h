#pragma once
#include "Renderable.h"
#include "Texture3D.h"

namespace GLEngine
{
	class EnvironmentMapLight :
		public Renderable
	{
	public:
		EnvironmentMapLight(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager);
		~EnvironmentMapLight();

		void SetGeometryTexture(Texture2D* geometryTexture) { _geometryTexture = geometryTexture; }
		void SetDiffuseTexture(Texture2D* diffuseTexture) { _diffuseTexture = diffuseTexture; }
		void SetSpecularRoughnessTexture(Texture2D* specularRoughnessTexture) { _specularRoughnessTexture = specularRoughnessTexture; }

		// The rendering of the environment map light assumes that the light rendering frame buffer is already bound.
		// A screen VAO also has to be bound before.
		// It will however still bind the different maps from the G-Buffer to it's shader.
		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		Texture2D* _environmentMap;
		Texture3D* _convolutedEnvironmentMap = nullptr;

		// G-Buffer textures (excepting the emissive, useless here), to be set before calling Render.
		Texture2D *_geometryTexture, *_diffuseTexture, *_specularRoughnessTexture;

		static const int _samplesNumber = 1000;
		static const int _convolutedWidth = 512;
		static const int _convolutedHeight = 256;
		static Texture3D* ConvoluteEnvironmentMap(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager);
	};

}
