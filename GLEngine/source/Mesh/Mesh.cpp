#include <stdafx.h>
#include <Mesh\Mesh.h>

namespace GLEngine
{
	Mesh::Mesh()
	{
	}


	Mesh::~Mesh()
	{
	}

	void Mesh::InitializeVao()
	{
		// Position, texture coordinates and normals VAO.
		if (GetHasTextureCoordinates() && GetHasNormals())
		{
			_vao = new VertexArrayObject(GetElementsList(), GetPositionsList(), GetNormalsList(), GetTextureCoordinatesList());
		}
		// Position, texture coordinates VAO.
		else if(GetHasTextureCoordinates())
		{
			_vao = new VertexArrayObject(GetElementsList(), GetPositionsList(), GetTextureCoordinatesList());
		}
		// Position, normals VAO.
		else if (GetHasNormals())
		{
			_vao = new VertexArrayObject(GetElementsList(), GetPositionsList(), GetNormalsList());
		}
		// Only positions VAO.
		else
		{
			throw new std::exception("VAO not implemented.");
		}
	}

	float Mesh::GetBoundingSphereRadius() const
	{
		if (_boundingSphereRadius <= 0)
		{
			ComputeBoundSphereRadius();
		}

		return _boundingSphereRadius;
	}
}