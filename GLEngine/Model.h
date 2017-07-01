#pragma once
#include "Renderable.h"
#include "SceneElement.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace GLEngine
{
	class Model :
		public Renderable, public SceneElement
	{
	public:
		Model();
		Model(Mesh* mesh);
		// Constructor for an instance of Model "resource".
		Model(Model* resource);
		~Model();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager);
		void RenderResource(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager, SceneNode* parentNode);

		Model* GetResource() const { return _resource; }

		VertexArrayObject* GetVao() { return _mesh->GetVao(); }

	private:
		Model* _resource = nullptr;
		Mesh* _mesh;
	};
}