#include "stdafx.h"
#include "EnvironmentMapSky.h"
#include "OBJMesh.h"
#include "OBJMesh.h"
#include "OBJLoader.h"

#include <iostream>


namespace GLEngine
{
	EnvironmentMapSky::EnvironmentMapSky(Mesh* sphereMesh, Texture2D* environmentMap)
		: Renderable(), _sphereMesh(sphereMesh), _environmentMap(environmentMap)
	{
	}


	EnvironmentMapSky::~EnvironmentMapSky()
	{
	}

	void EnvironmentMapSky::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void EnvironmentMapSky::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		ShaderProgram* shaderProgram = graphicsResourceManager->GetEnvmapShader();

		// Activate the Model's shader.
		shaderProgram->Use();

		// Set the world, view, projection matrices.
		PerspectiveCamera* currentCamera = sceneManager->GetCurrentCamera();

		shaderProgram->GetUniform("projection")->SetValue(currentCamera->GetProjection());
		shaderProgram->GetUniform("view")->SetValue(currentCamera->GetView());
		shaderProgram->GetUniform("world")->SetValue(&Matrix4::CreateTranslation(*currentCamera->GetPosition()));

		// Textures
		graphicsResourceManager->GetTextureManager()->AssignTextureToUnit(_environmentMap);
		shaderProgram->GetUniform("envmap")->SetValue((GLuint)_environmentMap->GetBoundUnit());

		_sphereMesh->GetVao()->Bind();
		{
			glDrawElements(GL_TRIANGLES, _sphereMesh->GetVao()->GetElementsCount(), GL_UNSIGNED_INT, 0);
		}
		_sphereMesh->GetVao()->UnBind();

		graphicsResourceManager->GetTextureManager()->FreeUnits();
	}
}