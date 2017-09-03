#pragma once
#include "Light.h"
namespace GLEngine
{
	class PointLight :
		public Light, public SceneElement
	{
	public:
		PointLight(float power, float range);
		~PointLight();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		float _power, _range;
	};
}