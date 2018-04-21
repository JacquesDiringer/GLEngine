#pragma once
#include "MeshLoader.h"

#include <Mesh\OBJMesh.h>

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API OBJLoader :
		public MeshLoader
	{
	public:
		OBJLoader();
		~OBJLoader();

	protected:
		virtual OBJMesh* InitFromFileContent(string content) const;
	};
}