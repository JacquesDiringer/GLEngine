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

		_pixelSize = GLEngineMath::Vector2(1 / (float)width, 1 / (float)height);

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
		GraphicsDeviceManager* graphicsDeviceManager = graphicsResourceManager->GetGraphicsDeviceManager();

		// Ghosts pass.

		// Get the ghost shader.
		ComputeShaderProgram* ghostingShader = graphicsResourceManager->GetComputeShader("C:\\Utils\\GLEngine\\resource\\Shaders\\ImageProcessing\\Ghosting");
		
		// Use it's program so that we can set uniforms.
		ghostingShader->Use();

		// Set the ouput texture size.
		ghostingShader->GetUniform("texSize")->SetValue(GLEngineMath::Vector2(_downscaledGhostTexture->GetWidth(), _downscaledGhostTexture->GetHeight()));
			
		// Set the input texture.
		ghostingShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

		// Load/find and set the lens flare chromatic aberration texture.
		Texture2D* lensChromaticAberrationTex = textureManager->GetTexture("C:\\Utils\\GLEngine\\resource\\Textures\\lensChromaticAberration.png");
		ghostingShader->GetUniform("lensChromaticAberrationTex")->SetValue(textureManager->AssignTextureToUnit(lensChromaticAberrationTex));

		// Set the mip level for ghosts fetching.
		ghostingShader->GetUniform("uMipLevelForGhosts")->SetValue(_mipLevelForGhosts);

		// Set the pixel size.
		ghostingShader->GetUniform("uPixelSize")->SetValue(_pixelSize);

		// Change the frame buffer to the one for the downscaled ghosts.
		textureManager->BindImageTexture(0, _downscaledGhostTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		// Dispatch the compute work groups.
		ghostingShader->UseAndDispatch(_downscaledGhostTexture->GetWidth()/32, _downscaledGhostTexture->GetHeight()/32, 1, *graphicsDeviceManager);


		// Bi axial blur X passes.

		// Get the blur shader.
		ComputeShaderProgram* blurXYShader = graphicsResourceManager->GetComputeShader("C:\\Utils\\GLEngine\\resource\\Shaders\\ImageProcessing\\BlurXY");

		// Use it's program so that we can set uniforms.
		blurXYShader->Use();

		// Set the ouput texture size.
		blurXYShader->GetUniform("texSize")->SetValue(GLEngineMath::Vector2(_bluxXTexture->GetWidth(), _bluxXTexture->GetHeight()));

		// Blur on the X axis pass.

		// Set the input texture.
		blurXYShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_downscaledGhostTexture));

		// Set the pixel size.
		blurXYShader->GetUniform("uPixelSize")->SetValue(_pixelSize);

		// Set the X samples axis.
		blurXYShader->GetUniform("uSamplingAxis")->SetValue(GLEngineMath::Vector2(2,0));

		// Set the samples count.
		blurXYShader->GetUniform("uSamplesCount")->SetValue(_blurSamplesCount);

		// Change the frame buffer to the one for the blur X.
		textureManager->BindImageTexture(0, _bluxXTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		// Dispatch the compute work groups.
		blurXYShader->UseAndDispatch(_bluxXTexture->GetWidth() / 32, _bluxXTexture->GetHeight() / 32, 1, *graphicsDeviceManager);


		// Blur on the Y axis pass.
		// Less uniform need to be set, since they already have been set on the X axis pass.

		// Set the input texture.
		blurXYShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_bluxXTexture));

		// Set the X samples axis.
		blurXYShader->GetUniform("uSamplingAxis")->SetValue(GLEngineMath::Vector2(0, 2));

		// Change the frame buffer to the one for the blur X.
		textureManager->BindImageTexture(0, _bluxYTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		// Dispatch the compute work groups.
		blurXYShader->UseAndDispatch(_bluxYTexture->GetWidth() / 32, _bluxYTexture->GetHeight() / 32, 1, *graphicsDeviceManager);


		// Combination pass with the actual rendering and the lens effects.

		// Get the combiner shader.
		ComputeShaderProgram* combinerShader = graphicsResourceManager->GetComputeShader("C:\\Utils\\GLEngine\\resource\\Shaders\\ImageProcessing\\Combiner");

		// Use it's program so that we can set uniforms.
		combinerShader->Use();

		// Set the ouput texture size.
		combinerShader->GetUniform("texSize")->SetValue(GLEngineMath::Vector2(_outputTexture->GetWidth(), _outputTexture->GetHeight()));

		// Bind the output image.
		textureManager->BindImageTexture(0, _outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		combinerShader->GetUniform("inputTex0")->SetValue(_inputTexture->GetBoundUnit());
		combinerShader->GetUniform("inputTex1")->SetValue(textureManager->AssignTextureToUnit(_bluxYTexture));

		// Dispatch the compute work groups.
		combinerShader->UseAndDispatch(_outputTexture->GetWidth() / 32, _outputTexture->GetHeight() / 32, 1, *graphicsDeviceManager);

		_inputTexture->SetFiltering(GL_NEAREST, GL_NEAREST);

		textureManager->FreeUnits();
	}
}