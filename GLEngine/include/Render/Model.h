#pragma once
#include <Render\Renderable.h>
#include <SceneGraph\SceneElement.h>
#include <Mesh\Mesh.h>
#include <ShaderProgram.h>
#include <Texture\Texture2D.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API Model :
		public Renderable, public SceneElement
	{
	public:
		Model(Mesh* mesh, Texture2D *diffuseTexture, Texture2D *specularTexture, Texture2D *roughnessTexture);
		// Constructor for an instance of Model "resource".
		Model(Model* resource);
		~Model();

		virtual void Accept(SceneElementVisitor* visitor);

		virtual void Render(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager) const;
		void RenderResource(SceneManager* sceneManager, GraphicsResourceManager* graphicsResourceManager, SceneNode* parentNode) const;

		Model* GetResource() const { return _resource; }

		VertexArrayObject* GetVao() { return _mesh->GetVao(); }

		Texture2D* GetDiffuse() { return _diffuseTexture; }
		Texture2D* GetSpecular() { return _specularTexture; }
		Texture2D* GetRoughness() { return _roughnessTexture; }

		virtual float GetBoundingSphereRadius();

	private:
		Model* _resource = nullptr;
		Mesh* _mesh;

		Texture2D *_diffuseTexture = nullptr;
		Texture2D *_specularTexture = nullptr;
		Texture2D *_roughnessTexture = nullptr;
	};
}