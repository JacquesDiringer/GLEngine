#pragma once

#include<string>
#include <vector>

#include <Math\Vector2.h>
#include <Math\Vector3.h>
#include <VertexArrayObject.h>

using std::string;
using std::vector;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API Mesh
	{
	public:
		Mesh();
		~Mesh();

		void InitializeVao();
		virtual void ComputeBoundSphereRadius() const = 0;

		VertexArrayObject* GetVao() { return _vao; }

		string GetPath() const { return _path; }
		void SetPath(const string path) { _path = path; }

		bool GetHasTextureCoordinates() const { return _hasTextureCoordinates; }
		void SetHasTextureCoordinates(const bool value) { _hasTextureCoordinates = value; }

		bool GetHasNormals() const { return _hasNormals; }
		void SetHasNormals(const bool value) { _hasNormals = value; }

		float GetBoundingSphereRadius() const;

		virtual vector<int> GetElementsList() const = 0;
		virtual vector<GLEngineMath::Vector3*> GetPositionsList() const = 0;
		virtual vector<GLEngineMath::Vector2*> GetTextureCoordinatesList() const = 0;
		virtual vector<GLEngineMath::Vector3*> GetNormalsList() const = 0;

	protected:
		// We consider that the bounding sphere shares the same center as the mesh.
		// This might not be a tightly fiting bounding sphere if the objects meshes are all away from the mesh origin.
		// On the other hand, this means that the bounding sphere position doesn't have to be recomputed when the object is rotated.
		mutable float _boundingSphereRadius = 0;

	private:
		VertexArrayObject* _vao;
		string _path;
		bool _hasTextureCoordinates = false, _hasNormals = false;
	};

}