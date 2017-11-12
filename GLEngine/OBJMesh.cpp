#include "stdafx.h"
#include "OBJMesh.h"

#include <sstream>

namespace GLEngine
{
	OBJMesh::OBJMesh()
	{
		_vertexCoords = vector<Vector3*>();
		_normals = vector<Vector3*>();
		_textureCoords = vector<Vector2*>();

		_vertexCoordTriangles = list<ObjTriangle*>();
		_normalTriangles = list<ObjTriangle*>();
		_textureCoordTriangles = list<ObjTriangle*>();

		_finalVerticesIndexMap = map<string, int>();
		_finalVerticesList = list<Vertex*>();
		_finalTriangles = list<ObjTriangle*>();
	}

	OBJMesh::~OBJMesh()
	{
	}

	list<int> OBJMesh::GetElementsList() const
	{
		list<int> result = list<int>();

		for each (ObjTriangle* currentTriangle in _finalTriangles)
		{
			result.push_back(currentTriangle->id0);
			result.push_back(currentTriangle->id1);
			result.push_back(currentTriangle->id2);
		}

		return result;
	}

	list<Vector3*> OBJMesh::GetPositionsList() const
	{
		list<Vector3*> result = list<Vector3*>();

		for each (Vertex* currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex->vertexCoords);
		}

		return result;
	}

	list<Vector2*> OBJMesh::GetTextureCoordinatesList() const
	{
		list<Vector2*> result = list<Vector2*>();

		for each (Vertex* currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex->textureCoords);
		}

		return result;
	}

	list<Vector3*> OBJMesh::GetNormalsList() const
	{
		list<Vector3*> result = list<Vector3*>();

		for each (Vertex* currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex->normals);
		}

		return result;
	}

	void OBJMesh::ComputeFinalVerticesAndTriangles()
	{
		// Iterators
		list<ObjTriangle*>::iterator textureCoordTrianglesIterator = _textureCoordTriangles.begin();
		list<ObjTriangle*>::iterator normalTrianglesIterator = _normalTriangles.begin();

		// Each vertex is created only if it is not unique in term of coordinates, texture coordinates, and normal.
		for each (ObjTriangle* currentVertexTriangle in _vertexCoordTriangles)
		{
			int finalVertexId0, finalVertexId1, finalVertexId2;

			if (GetHasTextureCoordinates() && GetHasNormals())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle->id0, (*textureCoordTrianglesIterator)->id0, (*normalTrianglesIterator)->id0);
				finalVertexId1 = AddVertex(currentVertexTriangle->id1, (*textureCoordTrianglesIterator)->id1, (*normalTrianglesIterator)->id1);
				finalVertexId2 = AddVertex(currentVertexTriangle->id2, (*textureCoordTrianglesIterator)->id2, (*normalTrianglesIterator)->id2);
			}
			else if (GetHasTextureCoordinates())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle->id0, (*textureCoordTrianglesIterator)->id0, -1);
				finalVertexId1 = AddVertex(currentVertexTriangle->id1, (*textureCoordTrianglesIterator)->id1, -1);
				finalVertexId2 = AddVertex(currentVertexTriangle->id2, (*textureCoordTrianglesIterator)->id2, -1);
			}
			else if (GetHasNormals())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle->id0, -1, (*normalTrianglesIterator)->id0);
				finalVertexId1 = AddVertex(currentVertexTriangle->id1, -1, (*normalTrianglesIterator)->id1);
				finalVertexId2 = AddVertex(currentVertexTriangle->id2, -1, (*normalTrianglesIterator)->id2);
			}
			else
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle->id0, -1, -1);
				finalVertexId1 = AddVertex(currentVertexTriangle->id1, -1, -1);
				finalVertexId2 = AddVertex(currentVertexTriangle->id2, -1, -1);
			}

			// Add the final vertice indexes to the final triangles list.
			_finalTriangles.push_back(new ObjTriangle(finalVertexId0, finalVertexId1, finalVertexId2));

			// Update the iterators
			if (GetHasTextureCoordinates())
			{
				textureCoordTrianglesIterator++;
			}

			if (GetHasNormals())
			{
				normalTrianglesIterator++;
			}
		}
	}

	int OBJMesh::AddVertex(const int vertexId, const int textureCoordsId, const int normalId)
	{
		string vertexName = ComputeVertexName(vertexId, textureCoordsId, normalId);

		// Check if this vertice (with the same position, texture coords and normal) already exists.
		map<string, int>::iterator finalVerticeIterator = _finalVerticesIndexMap.find(vertexName);

		int finalVerticeIndex;
		if (finalVerticeIterator != _finalVerticesIndexMap.end())
		{
			// If the vertice already exists, get it's ID from the map.
			finalVerticeIndex = (*finalVerticeIterator).second;
		}
		else
		{
			// Determine the attributes of the new vertex.
			Vector3* vertexCoordinates = _vertexCoords[vertexId - 1];
			Vector2* textureCoordinates = textureCoordsId != -1 ? _textureCoords[textureCoordsId - 1] : NULL;
			Vector3* normal = normalId != -1 ? _normals[normalId - 1] : NULL;

			// Add this vertex to the final vertex list.
			_finalVerticesList.push_back(new Vertex(vertexCoordinates, textureCoordinates, normal));

			// Keep track of it's index in the list according to it's name.
			finalVerticeIndex = _finalVerticesList.size() - 1;
			_finalVerticesIndexMap.insert(std::pair<string, int>(vertexName, int(finalVerticeIndex)));
		}

		return finalVerticeIndex;
	}

	string OBJMesh::ComputeVertexName(const int vertexId, const int textureCoordsId, const int normalId) const
	{
		std::stringstream name;
		name << vertexId << "/" << textureCoordsId << "/" << normalId;
		return name.str();
	}
}
