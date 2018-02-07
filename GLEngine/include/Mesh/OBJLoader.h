#pragma once
#include "MeshLoader.h"

#include <Mesh\OBJMesh.h>

namespace GLEngine
{
	class OBJLoader :
		public MeshLoader
	{
	public:
		OBJLoader();
		~OBJLoader();

	protected:
		virtual OBJMesh* InitFromFileContent(string content) const;
	};
}