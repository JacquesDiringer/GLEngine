#include <stdafx.h>
#include <Render\PostProcesses\LensPostProcess.h>
#include <Texture\Texture2DRGBA16F.h>

#include <cmath>
#include <algorithm>


namespace GLEngine
{
	LensPostProcess::LensPostProcess(int width, int height, TextureManager* textureManager)
		: PostProcess(width, height, textureManager)
	{
		_downscaledGhostTexture = new Texture2DRGBA16F(width, height, (void*)NULL);
		_bluxXTexture = new Texture2DRGBA16F(width, height, (void*)NULL);
		_bluxYTexture = new Texture2DRGBA16F(width, height, (void*)NULL);

		_pixelSize = Vector2(1 / (float)width, 1 / (float)height);

		// The level of mip map is calculated by taking the deepest level of mip map (log2) minus N.
		_mipLevelForGhosts = std::max(log2f(std::max(width, height)) - 7, 0.0f);
		_blurSamplesCount = 10;
	}


	LensPostProcess::~LensPostProcess()
	{
	}

	void LensPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		// Generate the mip maps on the rendered image.
		_inputTexture->GenerateMipMaps();
		// Change filtering, so that we can actually use the mip maps.
		_inputTexture->SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		FrameBufferManager* frameBufferManager = graphicsResourceManager->GetFrameBufferManager();

		const FrameBuffer* outputFrameBuffer = frameBufferManager->GetBoundFrameBuffer();
		if (frameBufferManager->IsDefaultFrameBufferBound())
		{
			outputFrameBuffer = nullptr;
		}

		// Bind the screen VAO.
		VertexArrayObject* screenVAO = graphicsResourceManager->GetScreenVAO();
		screenVAO->Bind();
		{

			// Ghosts pass.

			// Bind the ghost shader.
			ShaderProgram* ghostingShader = graphicsResourceManager->GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\ImageProcessing\\Ghosting.frag");
			ghostingShader->Use();
			{
				// Set the input texture.
				ghostingShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

				// Load/find and set the lens flare chromatic aberration texture.
				Texture2D* lensChromaticAberrationTex = textureManager->GetTexture("..\\GLEngine\\resource\\Textures\\lensChromaticAberration.png");
				ghostingShader->GetUniform("lensChromaticAberrationTex")->SetValue(textureManager->AssignTextureToUnit(lensChromaticAberrationTex));

				// Set the mip level for ghosts fetching.
				ghostingShader->GetUniform("uMipLevelForGhosts")->SetValue(_mipLevelForGhosts);

				// Set the pixel size.
				ghostingShader->GetUniform("uPixelSize")->SetValue(_pixelSize);

				// Change the frame buffer to the one for the downscaled ghosts.
				textureManager->BindImageTexture(0, _downscaledGhostTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB16F);

				// Clear it before we draw.
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}

			// Bi axial blur X passes.

			// Bind the blur shader.
			ShaderProgram* blurXYShader = graphicsResourceManager->GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\ImageProcessing\\BlurXY.frag");
			blurXYShader->Use();
			{
				// Blur on the X axis pass.

				// Set the input texture.
				blurXYShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_downscaledGhostTexture));

				// Set the pixel size.
				blurXYShader->GetUniform("uPixelSize")->SetValue(_pixelSize);

				// Set the X samples axis.
				blurXYShader->GetUniform("uSamplingAxis")->SetValue(Vector2(2,0));

				// Set the samples count.
				blurXYShader->GetUniform("uSamplesCount")->SetValue(_blurSamplesCount);

				// Change the frame buffer to the one for the blur X.
				textureManager->BindImageTexture(0, _bluxXTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB16F);

				// Clear it before we draw.
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);


				// Blur on the Y axis pass.
				// Less uniform need to be set, since they already have been set on the X axis pass.

				// Set the input texture.
				blurXYShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_bluxXTexture));

				// Set the X samples axis.
				blurXYShader->GetUniform("uSamplingAxis")->SetValue(Vector2(0, 2));

				// Change the frame buffer to the one for the blur X.
				textureManager->BindImageTexture(0, _bluxYTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB16F);

				// Clear it before we draw.
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}

			// Combination pass with the actual rendering and the lens effects.

			// Bind the combiner shader.
			ShaderProgram* combinerShader = graphicsResourceManager->GetPbrCombinerShader();
			combinerShader->Use();
			{
				// Bind the output frame buffer.
				frameBufferManager->Bind(outputFrameBuffer);

				combinerShader->GetUniform("emissiveGTexture")->SetValue(_inputTexture->GetBoundUnit());
				combinerShader->GetUniform("lightingTexture")->SetValue(textureManager->AssignTextureToUnit(_bluxYTexture));
				// Draw the surface.
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
		}
		screenVAO->UnBind();

		_inputTexture->SetFiltering(GL_NEAREST, GL_NEAREST);

		textureManager->FreeUnits();
	}
}