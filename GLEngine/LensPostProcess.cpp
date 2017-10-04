#include "stdafx.h"
#include "LensPostProcess.h"


namespace GLEngine
{
	LensPostProcess::LensPostProcess(int width, int height)
		: PostProcess(width, height)
	{
	}


	LensPostProcess::~LensPostProcess()
	{
	}

	void LensPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager)
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Bind the bloom shader.
		ShaderProgram* ghostingShader = graphicsResourceManager->GetShader("ScreenSpace.vert", "Ghosting.frag");
		ghostingShader->Use();
		{
			// Set the input texture.
			ghostingShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

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