#include "stdafx.h"
#include "PointLight.h"


namespace GLEngine
{
	PointLight::PointLight(Vector3 power, float range)
		: _power(power), _range(range)
	{
	}

	PointLight::PointLight(Vector3 power, float range, Texture2D* projectedTex)
		: _power(power), _range(range), _projectedTex(projectedTex)
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::Accept(SceneElementVisitor * visitor)
	{
		visitor->Visit(this);
	}

	void PointLight::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		SceneNode* parentNode = GetParentNode();

		if (parentNode != nullptr)
		{
			TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

			// Set the blending mode to accumulate each new light.
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			ShaderProgram* shaderProgram;

			if (_projectedTex == nullptr)
			{
				shaderProgram = graphicsResourceManager->GetShader("PointLight.vert", "PointLight.frag");
			}
			else
			{
				shaderProgram = graphicsResourceManager->GetShader("PointLight.vert", "PointLightProjection.frag");
				shaderProgram->GetUniform("projectedTex")->SetValue(
					textureManager->AssignTextureToUnit(_projectedTex));
			}

			shaderProgram->Use();

			// Set the pixel size, to be able to fetch in the G-Buffer
			Vector2* pixelSize = new Vector2(1 / (float)graphicsResourceManager->GetViewportWidth(), 1 / (float)graphicsResourceManager->GetViewportHeight());
			shaderProgram->GetUniform("pixelSize")->SetValue(pixelSize);

			// Bind the G-Buffer textures.
			shaderProgram->GetUniform("geometryGTexture")->SetValue(
				textureManager->AssignTextureToUnit(_geometryTexture));
			shaderProgram->GetUniform("diffuseGTexture")->SetValue(
				textureManager->AssignTextureToUnit(_diffuseTexture));
			shaderProgram->GetUniform("specularRoughnessGTexture")->SetValue(
				textureManager->AssignTextureToUnit(_specularRoughnessTexture));

			// Set the world, view, projection matrices.
			shaderProgram->GetUniform("projection")->SetValue(sceneManager->GetCurrentCamera()->GetProjection());
			shaderProgram->GetUniform("view")->SetValue(sceneManager->GetCurrentCamera()->GetView());
			Matrix4 scaledWorldTransformation = *parentNode->GetWorldTransformation() * Matrix4::CreateScale(_range);
			shaderProgram->GetUniform("world")->SetValue(scaledWorldTransformation);

			if (_projectedTex != nullptr)
			{
				Matrix4 iWorld = *parentNode->GetWorldTransformation();
				iWorld.InvertRT();
				shaderProgram->GetUniform("iWorld")->SetValue(iWorld);
			}

			// Set the inverse of the view.
			shaderProgram->GetUniform("cameraPosition")->SetValue(new Vector3(sceneManager->GetCurrentCamera()->GetIView()->Position()));

			// Set the light's power.
			shaderProgram->GetUniform("power")->SetValue(&_power);

			graphicsResourceManager->GetSphereVAO()->Bind();
			{
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetSphereVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			graphicsResourceManager->GetSphereVAO()->UnBind();

			graphicsResourceManager->GetTextureManager()->FreeUnits();

			delete pixelSize;

			// Disable blending.
			glDisable(GL_BLEND);
		}
	}
}