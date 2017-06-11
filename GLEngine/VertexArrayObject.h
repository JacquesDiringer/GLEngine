#pragma once

#include <list>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Vector2.h"
#include "Vector3.h"

using std::list;
using Math::Vector2;
using Math::Vector3;

namespace GLEngine
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals);
		VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector3*> tangents, list<Vector3*> biTangents);
		VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector2*> uvCoordinates);
		VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector2*> uvCoordinates);
		VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector3*> tangents, list<Vector3*> biTangents, list<Vector2*> uvCoordinates);
		~VertexArrayObject();

		int GetElementsCount() const { return _elementsCount; }

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