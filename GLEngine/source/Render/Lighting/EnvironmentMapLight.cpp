#include <stdafx.h>
#include <Render\Lighting\EnvironmentMapLight.h>
#include <FrameBuffer\RGB16FBuffer.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace GLEngine
{
	EnvironmentMapLight::EnvironmentMapLight(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager)
		: _environmentMap(environmentMap)
	{
		_convolutedEnvironmentMap = ConvoluteEnvironmentMap(_environmentMap, graphicsResourceManager);
	}


	EnvironmentMapLight::~EnvironmentMapLight()
	{
	}

	void EnvironmentMapLight::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		if (_convolutedEnvironmentMap != nullptr)
		{
			TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

			// Activate the envmap lighting shader.
			ShaderProgram* envmapLightShader = graphicsResourceManager->GetEnvmapLightShader();
			envmapLightShader->Use();
			{
				// Bind the G_Buffer textures.
				envmapLightShader->GetUniform("geometryGTexture")->SetValue(
					textureManager->AssignTextureToUnit(_geometryTexture));
				envmapLightShader->GetUniform("diffuseGTexture")->SetValue(
					textureManager->AssignTextureToUnit(_diffuseTexture));
				envmapLightShader->GetUniform("specularRoughnessGTexture")->SetValue(
					textureManager->AssignTextureToUnit(_specularRoughnessTexture));

				// Set the inverse of the view.
				envmapLightShader->GetUniform("iView")->SetValue(sceneManager->GetCurrentCamera()->GetIView());

				// Set the inverse of the projection.
				envmapLightShader->GetUniform("iProjection")->SetValue(sceneManager->GetCurrentCamera()->GetIProjection());

				// Bind the convoluted environment map.
				envmapLightShader->GetUniform("iblMap")->SetValue(
					textureManager->AssignTextureToUnit(_convolutedEnvironmentMap));

				graphicsResourceManager->GetScreenVAO()->Bind();
				{
					// Draw the envmap light quad covering the screen.
					glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
				}
				graphicsResourceManager->GetScreenVAO()->UnBind();

				graphicsResourceManager->GetTextureManager()->FreeUnits();
			}
		}
		else
		{
			throw new std::exception("The convoluted map should exist.");
		}
	}

	Texture3D* EnvironmentMapLight::ConvoluteEnvironmentMap(Texture2D* environmentMap, GraphicsResourceManager* graphicsResourceManager)
	{
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		list<Texture2D*> textures2DToAggregate = list<Texture2D*>();

		// Image processing frame buffer.
		GLuint imageProcessFrameBuffer;
		glGenFramebuffers(1, &imageProcessFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, imageProcessFrameBuffer);

		// Enable drawing in the lighting texture.
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Set the viewport size to the generated 2D texture size.
		glViewport(0, 0, _convolutedWidth, _convolutedHeight);

		// Set the blending mode to accumulate each new render.
		graphicsDeviceManager->SetBooleanState(GL_BLEND, true);
		glBlendFunc(GL_ONE, GL_ONE);

		// Activate the convolution shader.

		ShaderProgram* convolutionShader = graphicsResourceManager->GetEnvmapConvolutionShader();
		convolutionShader->Use();

		// Fetch vector in the envmap.
		GLEngineMath::Vector3 currentFetchVector = GLEngineMath::Vector3();

		// Bind the screen VAO.
		VertexArrayObject* screenVAO = graphicsResourceManager->GetScreenVAO();
		screenVAO->Bind();
		{
			for (float roughnessInput = 1.0f; roughnessInput > 0.0001f; roughnessInput -= 0.1f)
			{
				// This texture will hold the convoluted environment map for the current integration angle.
				RGB16FBuffer* convolutedEnvmap = new RGB16FBuffer(_convolutedWidth, _convolutedHeight, graphicsResourceManager->GetFrameBufferManager());

				// Check if the frame buffer is complete.
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					throw new std::exception("Convolution frame buffer incomplete.");
				}

				// Clear the frame buffer to render to.
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				convolutionShader->GetUniform("roughnessInput")->SetValue(roughnessInput);

				// Set the divisor that will allow to integrate properly respecting the number of samples and the integration angle.
				convolutionShader->GetUniform("divisor")->SetValue(15 / ((float)_samplesNumber));

				// Set the environment map texture.
				graphicsResourceManager->GetTextureManager()->AssignTextureToUnit(environmentMap);
				convolutionShader->GetUniform("envmap")->SetValue((GLuint)environmentMap->GetBoundUnit());

				for (int fetchIndex = 0; fetchIndex < _samplesNumber; fetchIndex++)
				{
					// Randomly set the values of the fetch vector, but don't forget to normalize it.
					currentFetchVector.X(rand() % 10000 - 5000);
					currentFetchVector.Y(rand() % 10000 - 5000);
					currentFetchVector.Z(rand() % 10000 - 5000);
					currentFetchVector.Normalize();

					convolutionShader->GetUniform("currentFetchVector")->SetValue(currentFetchVector);

					glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
				}

				// Free texture units from the environment map.
				graphicsResourceManager->GetTextureManager()->FreeUnits();

				textures2DToAggregate.push_front(convolutedEnvmap->GetBoundTexture());

				// TODO: Dispose convolutedEnvmap here
			}

			// This texture will hold the resized environment map for the 0 value integration angle.
			RGB16FBuffer* convolutedEnvmap = new RGB16FBuffer(_convolutedWidth, _convolutedHeight, graphicsResourceManager->GetFrameBufferManager());

			// Check if the frame buffer is complete.
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				throw new std::exception("Convolution frame buffer incomplete.");
			}

			// Clear the frame buffer to render to.
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Activate the combiner shader and trick into just resizing the texture.
			ShaderProgram* combinerShader = graphicsResourceManager->GetTextureDrawShader();
			combinerShader->Use();

			// Set the envmap texture as the emissive texture, that will only be added.
			graphicsResourceManager->GetTextureManager()->AssignTextureToUnit(environmentMap);
			combinerShader->GetUniform("inputTexture")->SetValue((GLuint)environmentMap->GetBoundUnit());

			glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);

			textures2DToAggregate.push_front(convolutedEnvmap->GetBoundTexture());

			// TODO: Dispose convolutedEnvmap here
		}
		screenVAO->UnBind();

		// Disable blending.
		graphicsDeviceManager->SetBooleanState(GL_BLEND, false);

		// Unbind and delete frame buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &imageProcessFrameBuffer);

		// Reset the original rendering viewport.
		glViewport(0, 0, graphicsResourceManager->GetViewportWidth(), graphicsResourceManager->GetViewportHeight());

		Texture3D* result = new Texture3D(textures2DToAggregate);

		// Free temporary 2D textures id's.
		for each (Texture2D* currentTextureToFree in textures2DToAggregate)
		{
			delete(currentTextureToFree);
		}
		textures2DToAggregate.clear();

		// Free texture units from the environment map.
		graphicsResourceManager->GetTextureManager()->FreeUnits();

		return result;
	}
}
