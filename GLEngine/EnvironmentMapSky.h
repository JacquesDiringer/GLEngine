#pragma once

#include "Renderable.h"
#include "SceneElement.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TextureManager.h"
#include "SceneManager.h"

namespace GLEngine
{
	class EnvironmentMapSky :
		public Renderable, public SceneElement
	{
	public:
		EnvironmentMapSky(Mesh* sphereMesh, string envMapPath, TextureManager* textureManager);
		~EnvironmentMapSky();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);

		Texture2D* GetTexture() { return _environmentMap; }

	private:
		Mesh* _sphereMesh;
		Texture2D* _environmentMap = nullptr;
	};
}