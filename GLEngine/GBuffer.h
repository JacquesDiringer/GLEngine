#pragma once
#include "FrameBuffer.h"
#include "Texture2D.h"

namespace GLEngine
{
	class GBuffer :
		public FrameBuffer
	{
	public:
		GBuffer(int width, int height);
		~GBuffer();

		Texture2D* GetGeomtryTexture() { return _geometryTexture; }
		Texture2D* GetDiffuseTexture() { return _diffuseTexture; }
		Texture2D* GetSpecularRoughnessTexture() { return _specularRoughnessTexture; }
		Texture2D* GetEmissiveTexture() { return _emissiveTexture; }

		GLuint GetDepthBuffer() const { return _depthBuffer; }

	private:
		// G-Buffer textures.
		Texture2D *_geometryTexture, *_diffuseTexture, *_specularRoughnessTexture, *_emissiveTexture;

		// Depth buffer id.
		GLuint _depthBuffer;
	};

}
