#pragma once

#include<string>
#include <vector>

#include <Math\Vector2.h>
#include <Math\Vector3.h>
#include <VertexArrayObject.h>

using std::string;
using std::vector;

using Math::Vector2;
using Math::Vector3;

namespace GLEngine
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void InitializeVao();

		VertexArrayObject* GetVao() { return _vao; }

		string GetPath() const { return _path; }
		void SetPath(const string path) { _path = path; }

		bool GetHasTextureCoordinates() const { return _hasTextureCoordinates; }
		void SetHasTextureCoordinates(const bool value) { _hasTextureCoordinates = value; }

		bool GetHasNormals() const { return _hasNormals; }
		void SetHasNormals(const bool value) { _hasNormals = value; }

		virtual vector<int> GetElementsList() const = 0;
		virtual vector<Vector3*> GetPositionsList() const = 0;
		virtual vector<Vector2*> GetTextureCoordinatesList() const = 0;
		virtual vector<Vector3*> GetNormalsList() const = 0;

	private:
		VertexArrayObject* _vao;
		string _path;
		bool _hasTextureCoordinates = false, _hasNormals = false;
	};

}