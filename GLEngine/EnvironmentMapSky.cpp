#include "stdafx.h"
#include "EnvironmentMapSky.h"
#include "OBJMesh.h"
#include "OBJMesh.h"
#include "OBJLoader.h"

#include <iostream>


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
		// Disable depth testing and set it.
		glDisable(GL_DEPTH_TEST);

		// Activate the Model's shader.
		_shaderProgram->Use();

		// Set the world, view, projection matrices.
		PerspectiveCamera* currentCamera = sceneManager->GetCurrentCamera();

		//std::cout << "Position : " << cameraPosition.X() << "; " << cameraPosition.Y() << "; " << cameraPosition.Z() << std::endl;

		_shaderProgram->GetUniform("projection")->SetValue(currentCamera->GetProjection());
		_shaderProgram->GetUniform("view")->SetValue(currentCamera->GetView());
		//_shaderProgram->GetUniform("world")->SetValue(&Matrix4::CreateTranslation(Vector3(cameraPosition.X(), cameraPosition.Y(), cameraPosition.Z())));
		_shaderProgram->GetUniform("world")->SetValue(&Matrix4::CreateTranslation(*currentCamera->GetPosition()));
		//_shaderProgram->GetUniform("view")->SetValue(new Matrix4());
		//_shaderProgram->GetUniform("world")->SetValue(new Matrix4());
			
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
}