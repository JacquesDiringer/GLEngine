#pragma once
#include "FrameBuffer.h"
#include "Texture2D.h"

namespace GLEngine
{
	class GBuffer :
		public FrameBuffer
	{
	public:
		GBuffer(int width, int height, FrameBufferManager* manager);
		~GBuffer();

		Texture2D* GetGeomtryTexture() const { return _geometryTexture; }
		Texture2D* GetDiffuseTexture() const { return _diffuseTexture; }
		Texture2D* GetSpecularRoughnessTexture() const { return _specularRoughnessTexture; }
		Texture2D* GetEmissiveTexture() const { return _emissiveTexture; }

		GLuint GetDepthBuffer() const { return _depthBuffer; }

	private:
		// G-Buffer textures.
		Texture2D *_geometryTexture, *_diffuseTexture, *_specularRoughnessTexture, *_emissiveTexture;

		// Depth buffer id.
		GLuint _depthBuffer;
	};

}
