#pragma once
#include "Light.h"
namespace GLEngine
{
	class PointLight :
		public Light, public SceneElement
	{
	public:
		PointLight(Vector3 power, float range);
		~PointLight();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

	private:
		Vector3 _power;
		float _range;
	};
}