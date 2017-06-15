#pragma once
#include "Renderable.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace GLEngine
{
	class Model :
		public Renderable
	{
	public:
		Model();
		Model(Mesh* mesh);
		~Model();

		void Render(SceneManager* sceneManager);

		void SetShaderProgram(ShaderProgram* shader) { _shaderProgram = shader; }


	private:
		Mesh* _mesh;
		ShaderProgram* _shaderProgram;
	};
}