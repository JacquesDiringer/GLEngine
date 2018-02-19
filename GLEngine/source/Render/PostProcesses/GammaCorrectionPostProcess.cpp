#include <stdafx.h>
#include <Render\PostProcesses\GammaCorrectionPostProcess.h>


namespace GLEngine
{
	GammaCorrectionPostProcess::GammaCorrectionPostProcess(int width, int height, TextureManager* textureManager)
		: PostProcess(width, height, textureManager)
	{
	}


	GammaCorrectionPostProcess::~GammaCorrectionPostProcess()
	{
	}

	void GammaCorrectionPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Bind the bloom shader.
		ShaderProgram* gammaCorrectionShader = graphicsResourceManager->GetShader("..\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "..\\GLEngine\\resource\\Shaders\\ImageProcessing\\GammaCorrection.frag");
		gammaCorrectionShader->Use();
		{
			// Set the input texture.
			gammaCorrectionShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

			// Bind the screen VAO.
			VertexArrayObject* screenVAO = graphicsResourceManager->GetScreenVAO();
			screenVAO->Bind();
			{
				glDrawElements(GL_TRIANGLES, screenVAO->GetElementsCount(), GL_UNSIGNED_INT, 0);
			}
			screenVAO->UnBind();
		}

		textureManager->FreeUnits();
	}
}
