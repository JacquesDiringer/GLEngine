#include "stdafx.h"
#include "VertexArrayObject.h"


namespace GLEngine
{
	VertexArrayObject::VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals)
	{
		_elementsCount = elements.size();

		if (positions.size() != normals.size())
		{
			throw new std::invalid_argument("Lists of vertex attributes should be the same size.");
		}

		// Sum of the elements contained in the differents attributes.
		int containedElements = 3 + 3;
		int size = containedElements * positions.size();
		GLsizei stride = containedElements * sizeof(GL_FLOAT);

		// Prepare the VBO data that will be given to the GPU.
		GLfloat* vboData = new GLfloat[size];

		list<Vector3*>::iterator positionIterator = positions.begin();
		list<Vector3*>::iterator normalIterator = normals.begin();

		for (int vertedId = 0; vertedId < positions.size(); vertedId++)
		{
			vboData[vertedId * containedElements] = (*positionIterator)->X();
			vboData[vertedId * containedElements + 1] = (*positionIterator)->Y();
			vboData[vertedId * containedElements + 2] = (*positionIterator)->Z();

			vboData[vertedId * containedElements + 3] = (*normalIterator)->X();
			vboData[vertedId * containedElements + 4] = (*normalIterator)->Y();
			vboData[vertedId * containedElements + 5] = (*normalIterator)->Z();

			positionIterator++;
			normalIterator++;
		}

		// VAO creation.
		GLuint VAO;
		glGenVertexArrays(1, &VAO);

		_vaoId = VAO;

		glBindVertexArray(VAO);

		// Vertex attributes linking to the VBO.
		//VBO creation.
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Setting the vertices
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vboData, GL_STATIC_DRAW);

		// Vertex position, location: 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Vertex normal, location: 2
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);


		// EBO

		// Prepare the EBO data that will be given to the GPU.
		GLuint* eboData = new GLuint[elements.size()];

		list<int>::iterator elementIterator = elements.begin();

		for (int elementId = 0; elementId < elements.size(); ++elementId)
		{
			eboData[elementId] = *elementIterator;
			elementIterator++;
		}


		GLuint EBO;
		glGenBuffers(1, &EBO);

		// Copy our index array in an element buffer for OpenGL to use.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), eboData, GL_STATIC_DRAW);

		// Unbing the VAO.
		glBindVertexArray(0);

		// Unbind the EBO, has to be done after the VAO is unbound.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Unbind the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexArrayObject::VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector3*> tangents, list<Vector3*> biTangents)
	{
		_elementsCount = elements.size();
	}

	VertexArrayObject::VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector2*> uvCoordinates)
	{
		_elementsCount = elements.size();

		if (positions.size() != uvCoordinates.size())
		{
			throw new std::invalid_argument("Lists of vertex attributes should be the same size.");
		}


		// Sum of the elements contained in the differents attributes.
		int containedElements = 3 + 2;
		int size = containedElements * positions.size();
		GLsizei stride = containedElements * sizeof(GL_FLOAT);

		// Prepare the VBO data that will be given to the GPU.
		GLfloat* vboData = new GLfloat[size];

		list<Vector3*>::iterator positionIterator = positions.begin();
		list<Vector2*>::iterator uvIterator = uvCoordinates.begin();

		for (int vertedId = 0; vertedId < positions.size(); vertedId++)
		{
			vboData[vertedId * containedElements] = (*positionIterator)->X();
			vboData[vertedId * containedElements + 1] = (*positionIterator)->Y();
			vboData[vertedId * containedElements + 2] = (*positionIterator)->Z();

			vboData[vertedId * containedElements + 3] = (*uvIterator)->X();
			vboData[vertedId * containedElements + 4] = (*uvIterator)->Y();
			positionIterator++;
			uvIterator++;
		}

		// VAO creation.
		GLuint VAO;
		glGenVertexArrays(1, &VAO);

		_vaoId = VAO;

		glBindVertexArray(VAO);

		// Vertex attributes linking to the VBO.
		//VBO creation.
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Setting the vertices
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vboData, GL_STATIC_DRAW);

		// Vertex position, location: 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Vertex uv, location: 1
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);


		// EBO

		// Prepare the EBO data that will be given to the GPU.
		GLuint* eboData = new GLuint[elements.size()];

		list<int>::iterator elementIterator = elements.begin();

		for (int elementId = 0; elementId < elements.size(); ++elementId)
		{
			eboData[elementId] = *elementIterator;
			elementIterator++;
		}


		GLuint EBO;
		glGenBuffers(1, &EBO);

		// Copy our index array in an element buffer for OpenGL to use.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), eboData, GL_STATIC_DRAW);

		// Unbing the VAO.
		glBindVertexArray(0);

		// Unbind the EBO, has to be done after the VAO is unbound.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Unbind the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexArrayObject::VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector2*> uvCoordinates)
	{
		_elementsCount = elements.size();

		if (positions.size() != uvCoordinates.size())
		{
			throw new std::invalid_argument("Lists of vertex attributes should be the same size.");
		}


		// Sum of the elements contained in the differents attributes.
		int containedElements = 3 + 2 + 3;
		int size = containedElements * positions.size();
		GLsizei stride = containedElements * sizeof(GL_FLOAT);

		// Prepare the VBO data that will be given to the GPU.
		GLfloat* vboData = new GLfloat[size];

		list<Vector3*>::iterator positionIterator = positions.begin();
		list<Vector2*>::iterator uvIterator = uvCoordinates.begin();
		list<Vector3*>::iterator normalIterator = normals.begin();

		for (int vertedId = 0; vertedId < positions.size(); vertedId++)
		{
			vboData[vertedId * containedElements] = (*positionIterator)->X();
			vboData[vertedId * containedElements + 1] = (*positionIterator)->Y();
			vboData[vertedId * containedElements + 2] = (*positionIterator)->Z();

			vboData[vertedId * containedElements + 3] = (*uvIterator)->X();
			vboData[vertedId * containedElements + 4] = (*uvIterator)->Y();

			vboData[vertedId * containedElements + 5] = (*normalIterator)->X();
			vboData[vertedId * containedElements + 6] = (*normalIterator)->Y();
			vboData[vertedId * containedElements + 7] = (*normalIterator)->Z();

			positionIterator++;
			uvIterator++;
			normalIterator++;
		}

		// VAO creation.
		GLuint VAO;
		glGenVertexArrays(1, &VAO);

		_vaoId = VAO;

		glBindVertexArray(VAO);

		// Vertex attributes linking to the VBO.
		//VBO creation.
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Setting the vertices
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vboData, GL_STATIC_DRAW);

		// Vertex position, location: 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Vertex uv, location: 1
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		// Vertex normal, location: 2
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)((3 + 2) * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);


		// EBO

		// Prepare the EBO data that will be given to the GPU.
		GLuint* eboData = new GLuint[elements.size()];

		list<int>::iterator elementIterator = elements.begin();

		for (int elementId = 0; elementId < elements.size(); ++elementId)
		{
			eboData[elementId] = *elementIterator;
			elementIterator++;
		}


		GLuint EBO;
		glGenBuffers(1, &EBO);

		// Copy our index array in an element buffer for OpenGL to use.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), eboData, GL_STATIC_DRAW);

		// Unbing the VAO.
		glBindVertexArray(0);

		// Unbind the EBO, has to be done after the VAO is unbound.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Unbind the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexArrayObject::VertexArrayObject(list<int> elements, list<Vector3*> positions, list<Vector3*> normals, list<Vector3*> tangents, list<Vector3*> biTangents, list<Vector2*> uvCoordinates)
	{
		_elementsCount = elements.size();
	}

	VertexArrayObject::~VertexArrayObject()
	{
	}
	void VertexArrayObject::Bind()
	{
		glBindVertexArray(_vaoId);
		_bound = true;
	}
	void VertexArrayObject::UnBind()
	{
		glBindVertexArray(0);
		_bound = false;
	}
}