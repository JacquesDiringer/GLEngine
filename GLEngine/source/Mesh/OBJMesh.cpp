#include <stdafx.h>
#include <Mesh\OBJMesh.h>

#include <sstream>

namespace GLEngine
{
	OBJMesh::OBJMesh()
	{
		_vertexCoords = vector<GLEngineMath::Vector3>();
		_normals = vector<GLEngineMath::Vector3>();
		_textureCoords = vector<GLEngineMath::Vector2>();

		_vertexCoordTriangles = vector<ObjTriangle>();
		_normalTriangles = vector<ObjTriangle>();
		_textureCoordTriangles = vector<ObjTriangle>();

		_finalVerticesIndexMap = unordered_map<string, int>();
		_finalVerticesList = vector<Vertex>();
		_finalTriangles = vector<ObjTriangle>();
	}

	OBJMesh::~OBJMesh()
	{
	}

	void OBJMesh::ComputeBoundSphereRadius() const
	{
		for (Vertex currentVertex : _finalVerticesList)
		{
			GLEngineMath::Vector3* vertexPosition = currentVertex.vertexCoords;

			float vertexLength = (*vertexPosition).Length();

			// The bounding sphere shares the same center than the center of the mesh.
			// Therefore, we can simply check if the current vertice is outside the sphere by comparing it's distance to the origin (i.e. it's length).
			if (vertexLength > _boundingSphereRadius)
			{
				_boundingSphereRadius = vertexLength;
			}
		}
	}

	vector<int> OBJMesh::GetElementsList() const
	{
		vector<int> result = vector<int>();

		for each (ObjTriangle currentTriangle in _finalTriangles)
		{
			result.push_back(currentTriangle.id0);
			result.push_back(currentTriangle.id1);
			result.push_back(currentTriangle.id2);
		}

		return result;
	}

	vector<GLEngineMath::Vector3*> OBJMesh::GetPositionsList() const
	{
		vector<GLEngineMath::Vector3*> result = vector<GLEngineMath::Vector3*>();

		for each (Vertex currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex.vertexCoords);
		}

		return result;
	}

	vector<GLEngineMath::Vector2*> OBJMesh::GetTextureCoordinatesList() const
	{
		vector<GLEngineMath::Vector2*> result = vector<GLEngineMath::Vector2*>();

		for each (Vertex currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex.textureCoords);
		}

		return result;
	}

	vector<GLEngineMath::Vector3*> OBJMesh::GetNormalsList() const
	{
		vector<GLEngineMath::Vector3*> result = vector<GLEngineMath::Vector3*>();

		for each (Vertex currentVertex in _finalVerticesList)
		{
			result.push_back(currentVertex.normals);
		}

		return result;
	}

	void OBJMesh::ComputeFinalVerticesAndTriangles()
	{
		// Iterators
		vector<ObjTriangle>::iterator textureCoordTrianglesIterator = _textureCoordTriangles.begin();
		vector<ObjTriangle>::iterator normalTrianglesIterator = _normalTriangles.begin();

		// Each vertex is created only if it is not unique in term of coordinates, texture coordinates, and normal.
		for each (ObjTriangle currentVertexTriangle in _vertexCoordTriangles)
		{
			int finalVertexId0, finalVertexId1, finalVertexId2;

			if (GetHasTextureCoordinates() && GetHasNormals())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle.id0, (textureCoordTrianglesIterator)->id0, (normalTrianglesIterator)->id0);
				finalVertexId1 = AddVertex(currentVertexTriangle.id1, (textureCoordTrianglesIterator)->id1, (normalTrianglesIterator)->id1);
				finalVertexId2 = AddVertex(currentVertexTriangle.id2, (textureCoordTrianglesIterator)->id2, (normalTrianglesIterator)->id2);
			}
			else if (GetHasTextureCoordinates())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle.id0, (textureCoordTrianglesIterator)->id0, -1);
				finalVertexId1 = AddVertex(currentVertexTriangle.id1, (textureCoordTrianglesIterator)->id1, -1);
				finalVertexId2 = AddVertex(currentVertexTriangle.id2, (textureCoordTrianglesIterator)->id2, -1);
			}
			else if (GetHasNormals())
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle.id0, -1, (normalTrianglesIterator)->id0);
				finalVertexId1 = AddVertex(currentVertexTriangle.id1, -1, (normalTrianglesIterator)->id1);
				finalVertexId2 = AddVertex(currentVertexTriangle.id2, -1, (normalTrianglesIterator)->id2);
			}
			else
			{
				// Add the vertices to the new vertices to the lists or retrieve them from them, if they already exist
				finalVertexId0 = AddVertex(currentVertexTriangle.id0, -1, -1);
				finalVertexId1 = AddVertex(currentVertexTriangle.id1, -1, -1);
				finalVertexId2 = AddVertex(currentVertexTriangle.id2, -1, -1);
			}

			// Add the final vertice indexes to the final triangles list.
			_finalTriangles.push_back(ObjTriangle(finalVertexId0, finalVertexId1, finalVertexId2));

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
		auto finalVerticeIterator = _finalVerticesIndexMap.find(vertexName);

		int finalVerticeIndex;
		if (finalVerticeIterator != _finalVerticesIndexMap.end())
		{
			// If the vertice already exists, get it's ID from the map.
			finalVerticeIndex = (*finalVerticeIterator).second;
		}
		else
		{
			// Determine the attributes of the new vertex.
			GLEngineMath::Vector3* vertexCoordinates = &_vertexCoords[vertexId - 1];
			GLEngineMath::Vector2* textureCoordinates = textureCoordsId != -1 ? &_textureCoords[textureCoordsId - 1] : NULL;
			GLEngineMath::Vector3* normal = normalId != -1 ? &_normals[normalId - 1] : NULL;

			// Add this vertex to the final vertex list.
			_finalVerticesList.push_back( Vertex(vertexCoordinates, textureCoordinates, normal));

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
