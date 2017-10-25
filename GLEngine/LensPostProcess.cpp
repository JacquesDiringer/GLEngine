#include "stdafx.h"
#include "LensPostProcess.h"


namespace GLEngine
{
	LensPostProcess::LensPostProcess(int width, int height, FrameBufferManager* frameBufferManager)
		: PostProcess(width, height, frameBufferManager)
	{
		_downscaledGhostBuffer = new RGB16FBuffer(width, height, frameBufferManager);
	}


	LensPostProcess::~LensPostProcess()
	{
	}

	void LensPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager)
	{
		// Generate the mip maps on the rendered image.
		_inputTexture->GenerateMipMaps();
		// Change filtering, so that we can actually use the mip maps.
		_inputTexture->SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		FrameBufferManager* frameBufferManager = graphicsResourceManager->GetFrameBufferManager();

		FrameBuffer* outputFrameBuffer = frameBufferManager->GetBoundFrameBuffer();
		if (frameBufferManager->IsDefaultFrameBufferBound())
		{
			outputFrameBuffer = nullptr;
		}

		// Bind the screen VAO.
		VertexArrayObject* screenVAO = graphicsResourceManager->GetScreenVAO();
		screenVAO->Bind();
		{

			// Bind the ghost shader.
			ShaderProgram* ghostingShader = graphicsResourceManager->GetShader("ScreenSpace.vert", "Ghosting.frag");
			ghostingShader->Use();
			{
				// Set the input texture.
				ghostingShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

				// Load/find and set the lens flare chromatic aberration texture.
				Texture2D* lensChromaticAberrationTex = textureManager->GetTexture("lensChromaticAberration.png");
				ghostingShader->GetUniform("lensChromaticAberrationTex")->SetValue(textureManager->AssignTextureToUnit(lensChromaticAberrationTex));

				// Change the frame buffer to the one for the downscaled ghosts.
				frameBufferManager->Bind(_downscaledGhostBuffer);

				// Clear it before we draw.
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}

			// Bind the combiner shader.
			ShaderProgram* combinerShader = graphicsResourceManager->GetPbrCombinerShader();
			combinerShader->Use();
			{
				// Bind the output frame buffer.
				frameBufferManager->Bind(outputFrameBuffer);

				combinerShader->GetUniform("emissiveGTexture")->SetValue(_inputTexture->GetBoundUnit());
				combinerShader->GetUniform("lightingTexture")->SetValue(textureManager->AssignTextureToUnit(_downscaledGhostBuffer->GetBoundTexture()));
				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
		}
		screenVAO->UnBind();

		_inputTexture->SetFiltering(GL_NEAREST, GL_NEAREST);

		textureManager->FreeUnits();
	}
}