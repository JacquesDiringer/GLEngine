#include "stdafx.h"
#include "EnvironmentMapSky.h"
#include "OBJMesh.h"
#include "OBJMesh.h"
#include "OBJLoader.h"


namespace GLEngine
{
	EnvironmentMapSky::EnvironmentMapSky(ShaderProgram* shaderProgram, Mesh* sphereMesh, string envMapPath, TextureManager* textureManager)
		: Renderable(), _shaderProgram(shaderProgram), _sphereMesh(sphereMesh), _textureManager(textureManager)
	{
		_environmentMap = textureManager->GetTexture(envMapPath);
	}


	EnvironmentMapSky::~EnvironmentMapSky()
	{
	}

	void EnvironmentMapSky::Render(SceneManager * sceneManager)
	{
		SceneNode* parentNode = GetParentNode();
		if (parentNode != nullptr)
		{
			// Enable depth testing.
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			// Activate the Model's shader.
			_shaderProgram->Use();

			// Set the world, view, projection matrices.
			_shaderProgram->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			_shaderProgram->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());
			_shaderProgram->GetUniform("world")->SetValue(parentNode->GetWorldTransformation());

			// Textures
			_textureManager->AssignTextureToUnit(_environmentMap);
			_shaderProgram->GetUniform("envmap")->SetValue((GLuint)_environmentMap->GetBoundUnit());


			_sphereMesh->GetVao()->Bind();
			{
				glDrawElements(GL_TRIANGLES, _sphereMesh->GetVao()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			_sphereMesh->GetVao()->UnBind();

			_textureManager->FreeUnits();
		}
		else
		{
			throw new std::exception("A Renderable must have a parent SceneNode to be rendered.");
		}
	}
}