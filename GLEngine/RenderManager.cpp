#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

namespace GLEngine
{
	RenderManager::RenderManager(int width, int height)
		: _viewportWidth(width), _viewportHeight(height)
	{
		_collectorVisitor = new RenderableCollectorVisitor();
		_modelsRenderQueue = new NoAlphaRenderQueue();
		_skyRenderQueue = new SkyRenderQueue();

		InitializeFrameBuffers();
	}


	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager)
	{
		double timeAtRenderStart = glfwGetTime();

		// Triggers renderable elements collection.
		sceneManager->GetRootNode()->Accept(_collectorVisitor);
		RenderingElements* collection = _collectorVisitor->GetCollectedElements();

		// Attach the G-Buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render elements in the right order.

		// Sky goes first.
		EnvironmentMapSky* sky = collection->GetSky();
		if (sky != nullptr)
		{
			_skyRenderQueue->AddRenderable(sky);

			_skyRenderQueue->Render(sceneManager, graphicsResourceManager);
		}

		// Then the models.
		// Fill the models render queue.
		for (InstancedModel* currentInstancedModel = collection->PopInstancedModel(); currentInstancedModel != nullptr; currentInstancedModel = collection->PopInstancedModel())
		{
			_modelsRenderQueue->AddRenderable(currentInstancedModel);
		}

		// Then render it.
		_modelsRenderQueue->Render(sceneManager, graphicsResourceManager);

		// Clear render queues.
		_skyRenderQueue->ClearRenderables();
		_modelsRenderQueue->ClearRenderables();

		// TODO: this call to clear will not stay necessary, each list should have had all it's elements popped at the end of the render loop.
		collection->Clear();

		// TODO : Move this in a clean function.
		// Deferred tests.

		graphicsResourceManager->GetScreenVAO()->Bind();
		{
			// Attach the lighting frame buffer.
			glBindFramebuffer(GL_FRAMEBUFFER, _lightingBuffer);

			// Activate the envmap lighting shader.
			ShaderProgram* envmapLightShader = graphicsResourceManager->GetEnvmapLightShader();
			envmapLightShader->Use();
			{
				// Bind the G_Buffer textures.
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _geometryTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, _specularRoughnessTexture);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, _emissiveTexture);

				// Give the texture units to the shader uniforms.
				envmapLightShader->GetUniform("geometryGTexture")->SetValue(0);
				envmapLightShader->GetUniform("diffuseGTexture")->SetValue(1);
				envmapLightShader->GetUniform("specularRoughnessGTexture")->SetValue(2);
				//envmapLightShader->GetUniform("emissiveGTexture")->SetValue(3);
				if (sky != nullptr)
				{
					Texture2D* skyTexture = sky->GetTexture();
					if (skyTexture != nullptr)
					{
						glActiveTexture(GL_TEXTURE4);
						//glBindTexture(GL_TEXTURE_2D, skyTexture->GetId());
						glBindTexture(GL_TEXTURE_2D, _iblTexture->GetId());
						envmapLightShader->GetUniform("envmap")->SetValue(4);
					}
				}

				// Set the inverse of the view.
				Matrix4 iView = Matrix4();
				iView.CopyFromMatrix4(sceneManager->GetCurrentCamera()->GetView());
				iView.InvertRT();
				envmapLightShader->GetUniform("iView")->SetValue(&iView);
				//envmapLightShader->GetUniform("cameraWorlPosition")->SetValue(&iView.Position());

				// Set the inverse of the projection.
				Matrix4 iProjection = Matrix4();
				iProjection.CopyFromMatrix4(sceneManager->GetCurrentCamera()->GetProjection());
				iProjection.Invert();
				envmapLightShader->GetUniform("iProjection")->SetValue(&iProjection);

				// Draw the envmap light quad covering the screen.
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}

			// Attach the default frame buffer.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Activate the PBR combiner shader.
			ShaderProgram* pbrCombinerShader = graphicsResourceManager->GetPbrCombinerShader();
			pbrCombinerShader->Use();
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, _lightingTexture);

				// Give the texture units to the shader uniforms, the g buffer units are still set from the lighting pass.
				//pbrCombinerShader->GetUniform("geometryGTexture")->SetValue(0);
				//pbrCombinerShader->GetUniform("diffuseGTexture")->SetValue(1);
				//pbrCombinerShader->GetUniform("specularRoughnessGTexture")->SetValue(2);
				pbrCombinerShader->GetUniform("emissiveGTexture")->SetValue(3);
				pbrCombinerShader->GetUniform("lightingTexture")->SetValue(4);

				// Draw the PBR combiner.
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
		}
		graphicsResourceManager->GetScreenVAO()->UnBind();


		double timeAtRenderEnd = glfwGetTime();

		double renderTime = timeAtRenderEnd - timeAtRenderStart;

		if (_frameCount%10 == 0)
		{
			std::cout << "Render time: " << renderTime * 1000 << "ms; FPS: " << 1/renderTime << std::endl;
		}

		++_frameCount;
	}

	void RenderManager::InitializeFrameBuffers()
	{
		// G-Buffer creation.
		glGenFramebuffers(1, &_gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

		// First bind a depth buffer.
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _viewportWidth, _viewportHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		

		// This texture will hold the (compressed) normal on RGB
		// The depth on the alpha.
		_geometryTexture;
		glGenTextures(1, &_geometryTexture);
		glBindTexture(GL_TEXTURE_2D, _geometryTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _viewportWidth, _viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _geometryTexture, 0);

		// This texture will hold the diffuse color on RGB
		// No alpha yet.
		_diffuseTexture;
		glGenTextures(1, &_diffuseTexture);
		glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 1.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _diffuseTexture, 0);

		// This texture will hold the specular color on RGB
		// The roughness on the alpha.
		_specularRoughnessTexture;
		glGenTextures(1, &_specularRoughnessTexture);
		glBindTexture(GL_TEXTURE_2D, _specularRoughnessTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _viewportWidth, _viewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 2.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _specularRoughnessTexture, 0);

		// This texture will hold the emissive color on RGB
		// No alpha yet.
		_emissiveTexture;
		glGenTextures(1, &_emissiveTexture);
		glBindTexture(GL_TEXTURE_2D, _emissiveTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 3.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _emissiveTexture, 0);

		// Enable drawing in these 4 color attachment
		GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

		// Check if the frame buffer is complete.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw new std::exception("G-Buffer frame buffer incomplete.");
		}


		// Lighting frame buffer creation.
		glGenFramebuffers(1, &_lightingBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _lightingBuffer);

		// This texture will hold the lighting value on RGB
		// No alpha yet.
		_lightingTexture;
		glGenTextures(1, &_lightingTexture);
		glBindTexture(GL_TEXTURE_2D, _lightingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _lightingTexture, 0);

		// Enable drawing in the lighting texture.
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Check if the frame buffer is complete.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw new std::exception("G-Buffer frame buffer incomplete.");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}