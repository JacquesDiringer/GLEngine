#include <stdafx.h>
#include <Render\PostProcesses\BloomPostProcess.h>
#include <ComputeShaderProgram.h>


namespace GLEngine
{
	BloomPostProcess::BloomPostProcess(int width, int height, TextureManager* textureManager)
		: PostProcess(width, height, textureManager)
	{
	}


	BloomPostProcess::~BloomPostProcess()
	{
	}

	void BloomPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();
		FrameBufferManager* frameBufferManager = graphicsResourceManager->GetFrameBufferManager();

		// Get the bloom shader.
		ComputeShaderProgram* bloomShader = graphicsResourceManager->GetComputeShader("..\\GLEngine\\resource\\Shaders\\ImageProcessing\\Bloom_ComputeShader");

		// Set the input texture.
		bloomShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

		// Bind the output texture as an image texture to the image unit 0, corresponding to binding=0 in the compute shader.
		BindInternalTextureAsOutput();
		//textureManager->BindImageTexture(0, _outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

		// Dispatch the compute work groups.
		bloomShader->UseAndDispatch(_outputTexture->GetWidth()/32, _outputTexture->GetHeight()/32, 1, *graphicsResourceManager->GetGraphicsDeviceManager());

		// Free texture units.
		textureManager->FreeUnits();
	}
}
