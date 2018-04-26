#include <stdafx.h>
#include <Mesh\MeshLoader.h>

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
		modelResult->InitializeVao();
		modelResult->ComputeBoundSphereRadius();

		return modelResult;
	}
}