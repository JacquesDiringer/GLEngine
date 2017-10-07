#include "stdafx.h"
#include "GammaCorrectionPostProcess.h"


namespace GLEngine
{
	GammaCorrectionPostProcess::GammaCorrectionPostProcess(int width, int height, FrameBufferManager* frameBufferManager)
		: PostProcess(width, height, frameBufferManager)
	{
	}


	GammaCorrectionPostProcess::~GammaCorrectionPostProcess()
	{
	}

	void GammaCorrectionPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager)
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Bind the bloom shader.
		ShaderProgram* gammaCorrectionShader = graphicsResourceManager->GetShader("ScreenSpace.vert", "GammaCorrection.frag");
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
