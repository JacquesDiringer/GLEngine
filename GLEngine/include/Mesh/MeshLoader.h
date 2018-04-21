#pragma once

#include<string>

#include <Mesh\Mesh.h>

using std::string;

#ifdef GLENGINE_EXPORTS
#define GLENGINE_API __declspec(dllexport)
#else
#define GLENGINE_API __declspec(dllimport)
#endif

namespace GLEngine
{
	class GLENGINE_API MeshLoader
	{
	public:
		MeshLoader();
		~MeshLoader();

		// Looks for a model file on disk, loads the file content, and uses it to initialize the corresponding model.
		Mesh* LoadModel(string path) const;

	protected:
		// Initializes the model corresponding to the content string.
		virtual Mesh* InitFromFileContent(string content) const = 0;
	};
}