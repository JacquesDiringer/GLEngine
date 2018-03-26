#include <stdafx.h>
#include <Render\Lighting\PointLight.h>


namespace GLEngine
{
	PointLight::PointLight(GLEngineMath::Vector3 power, float range)
		: _power(power), _range(range)
	{
	}

	PointLight::PointLight(GLEngineMath::Vector3 power, float range, Texture2D* projectedTex)
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
			GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();
			TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

			// Set the blending mode to accumulate each new light.
			graphicsDeviceManager->SetBooleanState(GL_BLEND, true);
			glBlendFunc(GL_ONE, GL_ONE);

			ShaderProgram* shaderProgram;

			if (_projectedTex == nullptr)
			{
				shaderProgram = graphicsResourceManager->GetShader("..\\GLEngine\\resource\\Shaders\\DeferredShading\\Lighting\\PointLight.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\Lighting\\PointLight.frag");
			}
			else
			{
				shaderProgram = graphicsResourceManager->GetShader("..\\GLEngine\\resource\\Shaders\\DeferredShading\\Lighting\\PointLight.vert", "..\\GLEngine\\resource\\Shaders\\DeferredShading\\Lighting\\PointLightProjection.frag");
				shaderProgram->GetUniform("projectedTex")->SetValue(
					textureManager->AssignTextureToUnit(_projectedTex));
			}

			shaderProgram->Use();

			// Set the pixel size, to be able to fetch in the G-Buffer
			GLEngineMath::Vector2 pixelSize = GLEngineMath::Vector2(1 / (float)graphicsResourceManager->GetViewportWidth(), 1 / (float)graphicsResourceManager->GetViewportHeight());
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
			GLEngineMath::Matrix4 scaledWorldTransformation = parentNode->GetWorldTransformation() * GLEngineMath::Matrix4::CreateScale(_range);
			shaderProgram->GetUniform("world")->SetValue(scaledWorldTransformation);

			if (_projectedTex != nullptr)
			{
				GLEngineMath::Matrix4 iWorld = parentNode->GetWorldTransformation();
				iWorld.InvertRT();
				shaderProgram->GetUniform("iWorld")->SetValue(iWorld);
			}

			// Set the inverse of the view.
			shaderProgram->GetUniform("cameraPosition")->SetValue(sceneManager->GetCurrentCamera()->GetIView().Position());

			// Set the light's power.
			shaderProgram->GetUniform("power")->SetValue(_power);

			graphicsResourceManager->GetSphereVAO()->Bind();
			{
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetSphereVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			graphicsResourceManager->GetSphereVAO()->UnBind();

			graphicsResourceManager->GetTextureManager()->FreeUnits();

			// Disable blending.
			graphicsDeviceManager->SetBooleanState(GL_BLEND, false);
		}
	}
}