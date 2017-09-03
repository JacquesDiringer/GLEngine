#pragma once

#include "Texture3D.h"
#include "Light.h"

namespace GLEngine
{
	class EnvironmentMapLight :
		public Light
	{
	public:
		EnvironmentMapLight(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager);
		~EnvironmentMapLight();

		// The rendering of the environment map light assumes that the light rendering frame buffer is already bound.
		// A screen VAO also has to be bound before.
		// It will however still bind the different maps from the G-Buffer to it's shader.
		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		Texture2D* _environmentMap;
		Texture3D* _convolutedEnvironmentMap = nullptr;

		static const int _samplesNumber = 1000;
		static const int _convolutedWidth = 512;
		static const int _convolutedHeight = 256;
		static Texture3D* ConvoluteEnvironmentMap(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager);
	};

}
