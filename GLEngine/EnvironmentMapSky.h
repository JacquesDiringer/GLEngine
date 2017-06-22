#pragma once

#include "Renderable.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "TextureManager.h"
#include "SceneManager.h"

namespace GLEngine
{
	class EnvironmentMapSky :
		public Renderable
	{
	public:
		EnvironmentMapSky(ShaderProgram* shaderProgram, Mesh* sphereMesh, string envMapPath, TextureManager* textureManager);
		~EnvironmentMapSky();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager);

	private:
		Mesh* _sphereMesh;
		ShaderProgram* _shaderProgram;
		Texture2D* _environmentMap;
		TextureManager* _textureManager;
	};
}