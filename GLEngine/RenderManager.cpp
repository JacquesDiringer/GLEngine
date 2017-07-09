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

		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

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

			if (_environmentMapLight == nullptr)
			{
				_environmentMapLight = new EnvironmentMapLight(sky->GetTexture(), graphicsResourceManager);
			}
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

			// Render the environment map PBR lighting.
			if (_environmentMapLight != nullptr)
			{
				// Set the needed G-Buffer maps first.
				_environmentMapLight->SetGeometryTexture(_geometryTexture);
				_environmentMapLight->SetDiffuseTexture(_diffuseTexture);
				_environmentMapLight->SetSpecularRoughnessTexture(_specularRoughnessTexture);

				// Render.
				_environmentMapLight->Render(sceneManager, graphicsResourceManager);
			}

			// Attach the default frame buffer.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Activate the PBR combiner shader.
			ShaderProgram* pbrCombinerShader = graphicsResourceManager->GetPbrCombinerShader();
			pbrCombinerShader->Use();
			{
				// Give the texture units to the shader uniforms, the g buffer units are still set from the lighting pass.
				pbrCombinerShader->GetUniform("emissiveGTexture")->SetValue(
					textureManager->AssignTextureToUnit(_emissiveTexture));
				pbrCombinerShader->GetUniform("lightingTexture")->SetValue(
					textureManager->AssignTextureToUnit(_lightingTexture));

				// Draw the PBR combiner.
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
		}
		graphicsResourceManager->GetScreenVAO()->UnBind();

		// Performance measurement.
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
		GLuint geometryTextureId;
		glGenTextures(1, &geometryTextureId);
		glBindTexture(GL_TEXTURE_2D, geometryTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _viewportWidth, _viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, geometryTextureId, 0);
		_geometryTexture = new Texture2D(geometryTextureId, _viewportWidth, _viewportHeight);

		// This texture will hold the diffuse color on RGB
		// No alpha yet.
		GLuint diffuseTextureId;
		glGenTextures(1, &diffuseTextureId);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 1.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuseTextureId, 0);
		_diffuseTexture = new Texture2D(diffuseTextureId, _viewportWidth, _viewportHeight);

		// This texture will hold the specular color on RGB
		// The roughness on the alpha.
		GLuint specularRoughnessTextureId;
		glGenTextures(1, &specularRoughnessTextureId);
		glBindTexture(GL_TEXTURE_2D, specularRoughnessTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _viewportWidth, _viewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 2.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specularRoughnessTextureId, 0);
		_specularRoughnessTexture = new Texture2D(specularRoughnessTextureId, _viewportWidth, _viewportHeight);

		// This texture will hold the emissive color on RGB
		// No alpha yet.
		GLuint emissiveTextureId;
		glGenTextures(1, &emissiveTextureId);
		glBindTexture(GL_TEXTURE_2D, emissiveTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 3.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, emissiveTextureId, 0);
		_emissiveTexture = new Texture2D(emissiveTextureId, _viewportWidth, _viewportHeight);

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
		GLuint lightingTextureId;
		glGenTextures(1, &lightingTextureId);
		glBindTexture(GL_TEXTURE_2D, lightingTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _viewportWidth, _viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Link the texture to the framebuffer color attachment 0.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingTextureId, 0);
		_lightingTexture = new Texture2D(lightingTextureId, _viewportWidth, _viewportHeight);
		
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