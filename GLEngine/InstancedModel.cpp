#include "stdafx.h"
#include "InstancedModel.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


namespace GLEngine
{
	InstancedModel::InstancedModel(Model* resource, vector<SceneNode*>* instancesNodes)
		: Renderable(), _resource(resource), _instancesNodes(instancesNodes)
	{
	}


	InstancedModel::~InstancedModel()
	{
		_instancesNodes->clear();
		delete _instancesNodes;
	}

	void InstancedModel::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager) const
	{
		if (_instancesNodes->size() < _instanciationThreshold)
		{
			// No instanciation.
			for each (SceneNode* currentNode in *_instancesNodes)
			{
				_resource->RenderResource(sceneManager, graphicsResourceManager, currentNode);
			}
		}
		else
		{
			// Instanciation.

			VBOManager* vboManager = graphicsResourceManager->GetVBOManager();

			// Concatenate all node matrices in one array.
			GLfloat* instanceMatricesArray = new GLfloat[16 * _instancesNodes->size()];
			int instanceMatricesArrayIndex = 0;
			for each (SceneNode* currentNode in *_instancesNodes)
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


			// Create or fetch the instances matrix VBO.
			GLsizeiptr requestedSize = _instancesNodes->size() * 16 * sizeof(GLfloat);
			GLuint instancesWorldVBO = vboManager->RequestBuffer(requestedSize);
			glBindBuffer(GL_ARRAY_BUFFER, instancesWorldVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, requestedSize, instanceMatricesArray);
			delete instanceMatricesArray;


			// Get the Model's shader.
			ShaderProgram* modelShader = graphicsResourceManager->GetModelPBRInstancedShader();

			// Activate the Model's shader.
			modelShader->Use();

			// Set the PBR textures.
			TextureManager* textureManager = graphicsResourceManager->GetTextureManager();
			textureManager->AssignTextureToUnit(_resource->GetDiffuse());
			modelShader->GetUniform("diffuseTexture")->SetValue(_resource->GetDiffuse()->GetBoundUnit());

			textureManager->AssignTextureToUnit(_resource->GetSpecular());
			modelShader->GetUniform("specularTexture")->SetValue(_resource->GetSpecular()->GetBoundUnit());

			textureManager->AssignTextureToUnit(_resource->GetRoughness());
			modelShader->GetUniform("roughnessTexture")->SetValue(_resource->GetRoughness()->GetBoundUnit());

			// Set the world, view, projection matrices.
			modelShader->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			modelShader->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());

			// Get and bind the VAO of the model.
			VertexArrayObject* resourceVao = _resource->GetVao();
			resourceVao->Bind();
			{
				resourceVao->EnableInstancingAttributes();
				// Draw all instances.
				glDrawElementsInstanced(GL_TRIANGLES, resourceVao->GetElementsCount(), GL_UNSIGNED_INT, 0, (GLsizei)_instancesNodes->size());

				// Unbind the instances VBO.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			resourceVao->UnBind();

			// Free the instances matrix VBO.
			vboManager->FreeBuffer(instancesWorldVBO);
		}
	}
}