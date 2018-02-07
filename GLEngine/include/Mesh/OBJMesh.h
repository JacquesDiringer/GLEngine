#pragma once

#include <unordered_map>
#include <vector>

#include <Mesh\Mesh.h>
#include <Math\Vector2.h>
#include <Math\Vector3.h>

using std::unordered_map;
using std::vector;

using Math::Vector2;
using Math::Vector3;

namespace GLEngine
{
	typedef struct ObjTriangle
	{
		int id0;
		int id1;
		int id2;

		ObjTriangle(int f0, int f1, int f2)
		{
			id0 = f0;
			id1 = f1;
			id2 = f2;
		}
	};

	typedef struct Vertex
	{
		Vector3* vertexCoords;
		Vector2* textureCoords;
		Vector3* normals;

		Vertex(Vector3* vC, Vector2* tC, Vector3* n)
		{
			vertexCoords = vC;
			textureCoords = tC;
			normals = n;
		}
	};

	class OBJMesh :
		public Mesh
	{
	public:
		OBJMesh();
		~OBJMesh();

		virtual vector<int> GetElementsList() const;
		virtual vector<Vector3*> GetPositionsList() const;
		virtual vector<Vector2*> GetTextureCoordinatesList() const;
		virtual vector<Vector3*> GetNormalsList() const;

		string GetObjectName() const { return _objectName; }
		void SetObjectName(const string objectName) { _objectName = objectName; }

		void AddVertexCoord(Vector3* vertexCoord) { _vertexCoords.push_back(vertexCoord); }
		void AddTextureCoord(Vector2* textureCoord) { _textureCoords.push_back(textureCoord); }
		void AddNormal(Vector3* normal) { _normals.push_back(normal); }

		void AddVertexCoordTriangle(ObjTriangle* triangle) { _vertexCoordTriangles.push_back(triangle); }
		void AddTextureCoordTriangle(ObjTriangle* triangle) { _textureCoordTriangles.push_back(triangle); }
		void AddNormalTriangle(ObjTriangle* triangle) { _normalTriangles.push_back(triangle); }

		// Computes a final vector of vertices and triangles from the "compressed" OBJ format.
		// These final lists are ready to be put in a VBO and EBO.
		void ComputeFinalVerticesAndTriangles();

	private:
		string _objectName;
		vector<Vector3*> _vertexCoords;
		vector<Vector2*> _textureCoords;
		vector<Vector3*> _normals;

		vector<ObjTriangle*> _vertexCoordTriangles;
		vector<ObjTriangle*> _textureCoordTriangles;
		vector<ObjTriangle*> _normalTriangles;

		unordered_map<string, int> _finalVerticesIndexMap;
		vector<Vertex*> _finalVerticesList;
		vector<ObjTriangle*> _finalTriangles;

		// Returns the final vertex ID
		int AddVertex(const int vertexId, const int textureCoordsId, const int normalId);
		string ComputeVertexName(const int vertexId, const int textureCoordsId, const int normalId) const;
	};
}