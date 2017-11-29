#pragma once

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Vector2.h"
#include "Vector3.h"

using std::vector;
using Math::Vector2;
using Math::Vector3;

namespace GLEngine
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject(vector<int> elements, vector<Vector3*> positions, vector<Vector3*> normals);
		VertexArrayObject(vector<int> elements, vector<Vector3*> positions, vector<Vector3*> normals, vector<Vector3*> tangents, vector<Vector3*> biTangents);
		VertexArrayObject(vector<int> elements, vector<Vector3*> positions, vector<Vector2*> uvCoordinates);
		VertexArrayObject(vector<int> elements, vector<Vector3*> positions, vector<Vector3*> normals, vector<Vector2*> uvCoordinates);
		VertexArrayObject(vector<int> elements, vector<Vector3*> positions, vector<Vector3*> normals, vector<Vector3*> tangents, vector<Vector3*> biTangents, vector<Vector2*> uvCoordinates);
		~VertexArrayObject();

		int GetElementsCount() const { return _elementsCount; }

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
		int _elementsCount;
	};
}