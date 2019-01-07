#include <stdafx.h>
#include <Render\PostProcesses\FogPostProcess.h>


namespace GLEngine
{
	FogPostProcess::FogPostProcess(int width, int height, TextureManager* textureManager, const RenderManager& renderManager)
		: PostProcess(width, height, textureManager)
	{
		_gBufferGeometryTexture = renderManager.GetGBufferGeometryTexture();
	}


	FogPostProcess::~FogPostProcess()
	{
	}

	void FogPostProcess::Render(SceneManager * sceneManager, GraphicsResourceManager * graphicsResourceManager) const
	{
		TextureManager* textureManager = graphicsResourceManager->GetTextureManager();

		// Bind the bloom shader.
		ShaderProgram* fogShader = graphicsResourceManager->GetShader("C:\\Utils\\GLEngine\\resource\\Shaders\\Common\\ScreenSpace.vert", "C:\\Utils\\GLEngine\\resource\\Shaders\\ImageProcessing\\Fog.frag");
		fogShader->Use();
		{
			// Set the input texture.
			fogShader->GetUniform("inputTex")->SetValue(textureManager->AssignTextureToUnit(_inputTexture));

			// Set the depth texture.
			fogShader->GetUniform("depthTex")->SetValue(textureManager->AssignTextureToUnit(_gBufferGeometryTexture));

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
