#pragma once

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <Math\Vector2.h>
#include <Math\Vector3.h>

using std::vector;

namespace GLEngine
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject(vector<int> elements, vector<GLEngineMath::Vector3*> positions, vector<GLEngineMath::Vector3*> normals);
		VertexArrayObject(vector<int> elements, vector<GLEngineMath::Vector3*> positions, vector<GLEngineMath::Vector3*> normals, vector<GLEngineMath::Vector3*> tangents, vector<GLEngineMath::Vector3*> biTangents);
		VertexArrayObject(vector<int> elements, vector<GLEngineMath::Vector3*> positions, vector<GLEngineMath::Vector2*> uvCoordinates);
		VertexArrayObject(vector<int> elements, vector<GLEngineMath::Vector3*> positions, vector<GLEngineMath::Vector3*> normals, vector<GLEngineMath::Vector2*> uvCoordinates);
		VertexArrayObject(vector<int> elements, vector<GLEngineMath::Vector3*> positions, vector<GLEngineMath::Vector3*> normals, vector<GLEngineMath::Vector3*> tangents, vector<GLEngineMath::Vector3*> biTangents, vector<GLEngineMath::Vector2*> uvCoordinates);
		~VertexArrayObject();

		int GetElementsCount() const { return _elementsCount; }
		int GetVertexCount() const { return _vertexCount; }

		// VertexArrayObject has to be bound before these twis function is called.
		// TODO: However, there is no tracking of what has been bound, this should change in the future
		void EnableInstancingAttributes();
		// VertexArrayObject has to be bound before these twis function is called.
		void DisableInstancingAttributes();

		// Binds the associated VAO id to the OpenGL context.
		void Bind();
		// Unbind the VAO by bindint the ID 0.
		void UnBind();

	private:
		GLuint _vaoId;
		bool _bound = false;
		int _elementsCount, _vertexCount;
	};
}