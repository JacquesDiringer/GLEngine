#pragma once

#include<string>
#include <list>

#include "Vector2.h"
#include "Vector3.h"

using std::string;
using std::list;

using Math::Vector2;
using Math::Vector3;

namespace GLEngine
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		string GetPath() const { return _path; }
		void SetPath(const string path) { _path = path; }

		bool GetHasTextureCoordinates() const { return _hasTextureCoordinates; }
		void SetHasTextureCoordinates(const bool value) { _hasTextureCoordinates = value; }

		bool GetHasNormals() const { return _hasNormals; }
		void SetHasNormals(const bool value) { _hasNormals = value; }

		virtual list<int> GetElementsList() = 0;
		virtual list<Vector3*> GetPositionsList() = 0;
		virtual list<Vector2*> GetTextureCoordinatesList() = 0;
		virtual list<Vector3*> GetNormalsList() = 0;

	private:
		string _path;
		bool _hasTextureCoordinates = false, _hasNormals = false;
	};

}