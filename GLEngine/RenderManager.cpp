#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"

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
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Triggers renderable elements collection.
		sceneManager->GetRootNode()->Accept(_collectorVisitor);
		RenderingElements* collection = _collectorVisitor->GetCollectedElements();

		// Attach the G-Buffer.
		_gBuffer->Bind();

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
			_lightingBuffer->Bind();

			// Render the environment map PBR lighting.
			if (_environmentMapLight != nullptr)
			{
				// Set the needed G-Buffer maps first.
				_environmentMapLight->SetGeometryTexture(_gBuffer->GetGeomtryTexture());
				_environmentMapLight->SetDiffuseTexture(_gBuffer->GetDiffuseTexture());
				_environmentMapLight->SetSpecularRoughnessTexture(_gBuffer->GetSpecularRoughnessTexture());

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
					textureManager->AssignTextureToUnit(_gBuffer->GetEmissiveTexture()));
				pbrCombinerShader->GetUniform("lightingTexture")->SetValue(
					textureManager->AssignTextureToUnit(_lightingBuffer->GetBoundTexture()));

				// Draw the PBR combiner.
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
		}
		graphicsResourceManager->GetScreenVAO()->UnBind();
	}

	void RenderManager::InitializeFrameBuffers()
	{
		_gBuffer = new GBuffer(_viewportWidth, _viewportHeight);

		_lightingBuffer = new RGB16FBuffer(_viewportWidth, _viewportHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}