#include "stdafx.h"
#include "Model.h"
#include "SceneNode.h"
#include "SceneManager.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


namespace GLEngine
{
	Model::Model()
	{
	}

	Model::Model(Mesh * mesh)
		: _mesh(mesh)
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

	void Model::Render(SceneManager * sceneManager)
	{
		if (_resource != nullptr)
		{
			_resource->RenderResource(sceneManager, GetParentNode());
		}
	}

	void Model::RenderResource(SceneManager * sceneManager, SceneNode * parentNode)
	{
		if (parentNode != nullptr)
		{
			// Activate the Model's shader.
			_shaderProgram->Use();

			// Set the world, view, projection matrices.
			_shaderProgram->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			_shaderProgram->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());
			_shaderProgram->GetUniform("world")->SetValue(parentNode->GetWorldTransformation());

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