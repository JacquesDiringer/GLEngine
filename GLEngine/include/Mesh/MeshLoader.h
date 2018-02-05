#pragma once

#include<string>

#include <Mesh\Mesh.h>

using std::string;

namespace GLEngine
{
	class MeshLoader
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