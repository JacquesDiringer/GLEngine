#include "stdafx.h"
#include "InstancedModel.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


namespace GLEngine
{
	InstancedModel::InstancedModel(Model* resource, list<SceneNode*> instancesNodes)
		: Renderable(), _resource(resource), _instancesNodes(instancesNodes)
	{
	}


	InstancedModel::~InstancedModel()
	{
		_instancesNodes.clear();
	}

	void InstancedModel::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		if (_instancesNodes.size() < _instanciationThreshold)
		{
			// No instanciation.
			for each (SceneNode* currentNode in _instancesNodes)
			{
				_resource->RenderResource(sceneManager, graphicsResourceManager, currentNode);
			}
		}
		else
		{
			// Instanciation.

			// Concatenate all node matrices in one array.
			GLfloat* instanceMatricesArray = new GLfloat[16 * _instancesNodes.size()];
			int instanceMatricesArrayIndex = 0;
			for each (SceneNode* currentNode in _instancesNodes)
			{
				GLfloat* worldArray = currentNode->GetWorldTransformation()->GetArray();

				for (int worldArrayIndex = 0; worldArrayIndex < 16; worldArrayIndex++)
				{
					int transposedIndex = 4 * (worldArrayIndex % 4) + floorf(worldArrayIndex / 4);
					instanceMatricesArray[16 * instanceMatricesArrayIndex + worldArrayIndex] = worldArray[transposedIndex];
				}

				delete worldArray; // will be deleted by the delete instanceMatricesArray ?
				++instanceMatricesArrayIndex;
			}


			// Create the instances matrix VBO.
			GLuint instancesWorldVBO;
			glGenBuffers(1, &instancesWorldVBO);
			glBindBuffer(GL_ARRAY_BUFFER, instancesWorldVBO);
			//glBufferData(GL_ARRAY_BUFFER, _instancesNodes.size() * 16 * sizeof(GLfloat), instanceMatricesArray, GL_STATIC_DRAW);
			// TODO: compare performances with the STREAM usage.
			glBufferData(GL_ARRAY_BUFFER, _instancesNodes.size() * 16 * sizeof(GLfloat), instanceMatricesArray, GL_STREAM_DRAW);
			delete instanceMatricesArray;

			VertexArrayObject* resourceVao = _resource->GetVao();

			// Get the Model's shader.
			ShaderProgram* modelShader = graphicsResourceManager->GetLambertianInstancedShader();

			// Activate the Model's shader.
			modelShader->Use();

			// Set the view, projection matrices.
			modelShader->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			modelShader->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());

			resourceVao->Bind();
			{
				resourceVao->EnableInstancingAttributes();
				glDrawElementsInstanced(GL_TRIANGLES, resourceVao->GetElementsCount(), GL_UNSIGNED_INT, 0, (GLsizei)_instancesNodes.size());

				// Unbind the instances VBO.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			resourceVao->UnBind();

			// Free the instances matrix VBO.
			glDeleteBuffers(1, &instancesWorldVBO);
		}
	}
}