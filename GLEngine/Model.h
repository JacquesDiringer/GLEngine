#pragma once
#include "Renderable.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace GLEngine
{
	class Model :
		public Renderable
	{
	public:
		Model();
		Model(Mesh* mesh);
		// Constructor for an instance of Model "resource".
		Model(Model* resource);
		~Model();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager);
		void RenderResource(SceneManager* sceneManager, SceneNode* parentNode);

		void SetShaderProgram(ShaderProgram* shader) { _shaderProgram = shader; }

	private:
		Model* _resource = nullptr;
		Mesh* _mesh;
		ShaderProgram* _shaderProgram;
	};
}