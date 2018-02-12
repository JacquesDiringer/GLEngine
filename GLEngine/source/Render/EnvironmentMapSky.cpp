#include <stdafx.h>
#include <Render\EnvironmentMapSky.h>
#include <Mesh\OBJMesh.h>
#include <Mesh\OBJLoader.h>

#include <iostream>


namespace GLEngine
{
	EnvironmentMapSky::EnvironmentMapSky(Texture2D* environmentMap)
		: Renderable(), _environmentMap(environmentMap)
	{
	}


	EnvironmentMapSky::~EnvironmentMapSky()
	{
	}

	void EnvironmentMapSky::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void EnvironmentMapSky::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager) const
	{
		ShaderProgram* shaderProgram = graphicsResourceManager->GetEnvmapShader();

		// Activate the Model's shader.
		shaderProgram->Use();

		// Set the world, view, projection matrices.
		PerspectiveCamera* currentCamera = sceneManager->GetCurrentCamera();

		shaderProgram->GetUniform("projection")->SetValue(currentCamera->GetProjection());
		shaderProgram->GetUniform("view")->SetValue(currentCamera->GetView());
		shaderProgram->GetUniform("world")->SetValue(Matrix4::CreateTranslation(currentCamera->GetPosition()));

		// Textures
		graphicsResourceManager->GetTextureManager()->AssignTextureToUnit(_environmentMap);
		shaderProgram->GetUniform("envmap")->SetValue((GLuint)_environmentMap->GetBoundUnit());

		graphicsResourceManager->GetSphereVAO()->Bind();
		{
			glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetSphereVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
		}
		graphicsResourceManager->GetSphereVAO()->UnBind();

		graphicsResourceManager->GetTextureManager()->FreeUnits();
	}
}