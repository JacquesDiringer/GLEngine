#pragma once

#include <unordered_map>
#include <vector>

#include <Mesh\Mesh.h>
#include <Math\Vector2.h>
#include <Math\Vector3.h>

using std::unordered_map;
using std::vector;

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
		GLEngineMath::Vector3* vertexCoords;
		GLEngineMath::Vector2* textureCoords;
		GLEngineMath::Vector3* normals;

		Vertex(GLEngineMath::Vector3* vC, GLEngineMath::Vector2* tC, GLEngineMath::Vector3* n)
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
		virtual vector<GLEngineMath::Vector3*> GetPositionsList() const;
		virtual vector<GLEngineMath::Vector2*> GetTextureCoordinatesList() const;
		virtual vector<GLEngineMath::Vector3*> GetNormalsList() const;

		string GetObjectName() const { return _objectName; }
		void SetObjectName(const string objectName) { _objectName = objectName; }

		void AddVertexCoord(GLEngineMath::Vector3 vertexCoord) { _vertexCoords.push_back(vertexCoord); }
		void AddTextureCoord(GLEngineMath::Vector2 textureCoord) { _textureCoords.push_back(textureCoord); }
		void AddNormal(GLEngineMath::Vector3 normal) { _normals.push_back(normal); }

		void AddVertexCoordTriangle(ObjTriangle triangle) { _vertexCoordTriangles.push_back(triangle); }
		void AddTextureCoordTriangle(ObjTriangle triangle) { _textureCoordTriangles.push_back(triangle); }
		void AddNormalTriangle(ObjTriangle triangle) { _normalTriangles.push_back(triangle); }

		// Computes a final vector of vertices and triangles from the "compressed" OBJ format.
		// These final lists are ready to be put in a VBO and EBO.
		void ComputeFinalVerticesAndTriangles();

	private:
		string _objectName;
		vector<GLEngineMath::Vector3> _vertexCoords;
		vector<GLEngineMath::Vector2> _textureCoords;
		vector<GLEngineMath::Vector3> _normals;

		vector<ObjTriangle> _vertexCoordTriangles;
		vector<ObjTriangle> _textureCoordTriangles;
		vector<ObjTriangle> _normalTriangles;

		unordered_map<string, int> _finalVerticesIndexMap;
		vector<Vertex> _finalVerticesList;
		vector<ObjTriangle> _finalTriangles;

		// Returns the final vertex ID
		int AddVertex(const int vertexId, const int textureCoordsId, const int normalId);
		string ComputeVertexName(const int vertexId, const int textureCoordsId, const int normalId) const;
	};
}