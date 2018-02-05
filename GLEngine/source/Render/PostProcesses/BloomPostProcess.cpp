#include <stdafx.h>
#include <Render\PostProcesses\BloomPostProcess.h>


namespace GLEngine
{
	BloomPostProcess::BloomPostProcess(int width, int height, FrameBufferManager* frameBufferManager)
		: PostProcess(width, height, frameBufferManager)
	{
	}


	BloomPostProcess::~BloomPostProcess()
	{
	}

	void BloomPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Bind the bloom shader.
		ShaderProgram* bloomShader = graphicsResourceManager->GetShader("ScreenSpace.vert", "Bloom.frag");
		bloomShader->Use();
		{
			// Set the input texture.
			bloomShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

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
