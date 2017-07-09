#include "stdafx.h"
#include "Model.h"
#include "SceneNode.h"
#include "SceneManager.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


namespace GLEngine
{
	Model::Model(Mesh * mesh, Texture2D *diffuseTexture, Texture2D *specularTexture, Texture2D *roughnessTexture)
		: _mesh(mesh), _diffuseTexture(diffuseTexture), _specularTexture(specularTexture), _roughnessTexture(roughnessTexture)
	{
	}

	Model::Model(Model * resource)
		: _resource(resource)
	{
	}

	Model::~Model()
	{
	}

	void Model::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void Model::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		if (_resource != nullptr)
		{
			_resource->RenderResource(sceneManager, graphicsResourceManager, GetParentNode());
		}
	}

	void Model::RenderResource(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager, SceneNode * parentNode)
	{
		if (parentNode != nullptr)
		{
			// Get the Model's shader.
			ShaderProgram* modelShader = graphicsResourceManager->GetModelPBRShader();

			// Activate the Model's shader.
			modelShader->Use();

			// Set the PBR textures.
			TextureManager* textureManager = graphicsResourceManager->GetTextureManager();
			textureManager->AssignTextureToUnit(_diffuseTexture);
			modelShader->GetUniform("diffuseTexture")->SetValue(_diffuseTexture->GetBoundUnit());

			textureManager->AssignTextureToUnit(_specularTexture);
			modelShader->GetUniform("specularTexture")->SetValue(_specularTexture->GetBoundUnit());

			textureManager->AssignTextureToUnit(_roughnessTexture);
			modelShader->GetUniform("roughnessTexture")->SetValue(_roughnessTexture->GetBoundUnit());

			// Set the world, view, projection matrices.
			modelShader->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			modelShader->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());
			modelShader->GetUniform("world")->SetValue(parentNode->GetWorldTransformation());

			_mesh->GetVao()->Bind();
			{
				glDrawElements(GL_TRIANGLES, _mesh->GetVao()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			_mesh->GetVao()->UnBind();
		}
		else
		{
			throw new std::exception("A Renderable must have a parent SceneNode to be rendered.");
		}
	}
}