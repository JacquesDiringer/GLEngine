#include "stdafx.h"
#include "RenderManager.h"
#include "RenderableCollectorVisitor.h"
#include "PostProcess.h"
#include "PointLight.h"

#include <iostream>

namespace GLEngine
{
	RenderManager::RenderManager(int width, int height, GraphicsResourceManager* graphicsResourceManager)
		: _viewportWidth(width), _viewportHeight(height)
	{
		_collectorVisitor = new RenderableCollectorVisitor();
		_modelsRenderQueue = new NoAlphaRenderQueue();
		_pointLightsRenderQueue = new PointLightRenderQueue();
		_skyRenderQueue = new SkyRenderQueue();

		InitializeFrameBuffers(graphicsResourceManager);
	}


	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Render(SceneManager * sceneManager, GraphicsResourceManager* graphicsResourceManager) const
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Triggers renderable elements collection.
		sceneManager->GetRootNode()->Accept(_collectorVisitor);
		RenderingElements* collection = _collectorVisitor->GetCollectedElements();

		// Attach the G-Buffer.
		_gBuffer->Bind();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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


		// TODO : Move these lighting passes in a clean function.

		// Attach the lighting frame buffer.
		_lightingBuffer->Bind();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Only the color has to be cleared, we need the depth rendered during the G-Buffer pass and it's render buffer id is shared with the framebuffer of the G-Buffer.
		glClear(GL_COLOR_BUFFER_BIT);

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

		for (PointLight* currentPointLight = collection->PopPointLight(); currentPointLight != nullptr; currentPointLight = collection->PopPointLight())
		{
			// Set the needed G-Buffer maps first.
			currentPointLight->SetGeometryTexture(_gBuffer->GetGeomtryTexture());
			currentPointLight->SetDiffuseTexture(_gBuffer->GetDiffuseTexture());
			currentPointLight->SetSpecularRoughnessTexture(_gBuffer->GetSpecularRoughnessTexture());

			_pointLightsRenderQueue->AddRenderable(currentPointLight);
		}

		_pointLightsRenderQueue->Render(sceneManager, graphicsResourceManager);

		// Get the post processes, if any.
		vector<PostProcess*> postProcesses = sceneManager->GetCurrentCamera()->GetPostProcesses();

		int postProcessesCount = postProcesses.size();
		if (postProcessesCount == 0)
		{
			// When there are no post processes,
			// Attach the default frame buffer, to have the final render call on screen.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			// When there are post processes,
			// Bind the combine frame buffer.
			_combineBuffer->Bind();
		}

		// Combine rendering.
		// Activate the PBR combiner shader.
		ShaderProgram* pbrCombinerShader = graphicsResourceManager->GetPbrCombinerShader();
		pbrCombinerShader->Use();
		{
			// Give the texture units to the shader uniforms, the g buffer units are still set from the lighting pass.
			pbrCombinerShader->GetUniform("emissiveGTexture")->SetValue(
				textureManager->AssignTextureToUnit(_gBuffer->GetEmissiveTexture()));
			pbrCombinerShader->GetUniform("lightingTexture")->SetValue(
				textureManager->AssignTextureToUnit(_lightingBuffer->GetBoundTexture()));

			graphicsResourceManager->GetScreenVAO()->Bind();
			{
				// Draw the PBR combiner.
				glDrawElements(GL_TRIANGLES, graphicsResourceManager->GetScreenVAO()->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			graphicsResourceManager->GetScreenVAO()->UnBind();

			textureManager->FreeUnits();
		}


		// Post processes rendering.
		int currentPostProcessId = 0;
		Texture2D* previousPostProcessResult;
		for each (PostProcess* currentPostProcess in postProcesses)
		{
			if (currentPostProcessId == 0)
			{
				// If this is the first post process being computed, use the combine frame buffer as the input texture.
				currentPostProcess->SetInputTexture(_combineBuffer->GetBoundTexture());
			}
			else
			{
				// Otherwise, take the result of the previous post process.
				currentPostProcess->SetInputTexture(previousPostProcessResult);
			}

			if (currentPostProcessId < postProcessesCount - 1)
			{
				currentPostProcess->BindFrameBuffer();
			}
			else
			{
				// When it is the last post process to be computed,
				// Attach the default frame buffer.
				graphicsResourceManager->GetFrameBufferManager()->SetDefaultFrameBuffer();
			}

			// Actually compute the post process.
			currentPostProcess->Render(sceneManager, graphicsResourceManager);

			// In case we need to pass it to the next one, remember the result.
			previousPostProcessResult = currentPostProcess->GetProcessedResult();

			++currentPostProcessId;
		}
		graphicsResourceManager->GetScreenVAO()->UnBind();

		// Clearing all rendering temporary collections.
		// Clear render queues.
		_skyRenderQueue->ClearRenderables();
		_modelsRenderQueue->ClearRenderables();
		_pointLightsRenderQueue->ClearRenderables();

		// Clear the elements in the collector
		_collectorVisitor->CheckAndResetCollection();
	}

	void RenderManager::InitializeFrameBuffers(GraphicsResourceManager* graphicsResourceManager)
	{
		_gBuffer = new GBuffer(_viewportWidth, _viewportHeight, graphicsResourceManager->GetFrameBufferManager());
		_lightingBuffer = new RGB16FBuffer(_viewportWidth, _viewportHeight, _gBuffer->GetDepthBuffer(), graphicsResourceManager->GetFrameBufferManager());
		_combineBuffer = new RGB16FBuffer(_viewportWidth, _viewportHeight, graphicsResourceManager->GetFrameBufferManager());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}