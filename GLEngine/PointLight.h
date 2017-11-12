#pragma once
#include "Light.h"
namespace GLEngine
{
	class PointLight :
		public Light, public SceneElement
	{
	public:
		PointLight(Vector3 power, float range);
		PointLight(Vector3 power, float range, Texture2D* projectedTex);
		~PointLight();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;

	private:
		Vector3 _power;
		float _range;
		Texture2D* _projectedTex = nullptr;
	};
}