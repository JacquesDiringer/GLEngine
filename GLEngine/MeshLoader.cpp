#include "stdafx.h"
#include "MeshLoader.h"

#include "Utils.h"

namespace GLEngine
{
	MeshLoader::MeshLoader()
	{
	}

	MeshLoader::~MeshLoader()
	{
	}
	Mesh * MeshLoader::LoadModel(string path) const
	{
		Mesh* modelResult = InitFromFileContent(Utils::ReadFile(path));
		modelResult->SetPath(path);

		return modelResult;
	}
}