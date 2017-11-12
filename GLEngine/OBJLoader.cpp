#include "stdafx.h"
#include "OBJLoader.h"

#include <iostream>
#include <sstream>

namespace GLEngine
{
	OBJLoader::OBJLoader()
	{
	}

	OBJLoader::~OBJLoader()
	{
	}

	OBJMesh * OBJLoader::InitFromFileContent(string content) const
	{
		OBJMesh* objModel = new OBJMesh();

		std::istringstream fileStream(content);

		std::string currentLine;
		while (std::getline(fileStream, currentLine))
		{
			if (currentLine[0] != '#')
			{
				if (currentLine[0] == 'o')
				{
					// Object name
					char objectName[50];
					sscanf(currentLine.c_str(), "%*s %s", objectName);

					objModel->SetObjectName(string(objectName));
				}
				else if (currentLine[0] == 'v')
				{
					// Vertices
					char vertexTypeChar[20];
					float x, y, z;
					sscanf(currentLine.c_str(), "%s %f %f %f", vertexTypeChar, &x, &y, &z);

					string vertexType(vertexTypeChar);
					if (vertexType == "v")
					{
						objModel->AddVertexCoord(new Vector3(x, y, z));
					}
					else if (vertexType == "vn")
					{
						objModel->AddNormal(new Vector3(x, y, z));
					}
					else if (vertexType == "vt")
					{
						objModel->AddTextureCoord(new Vector2(x, y));
					}
				}
				else if (currentLine[0] == 'f')
				{
					// Triangle/Rectangle, face.

					int v0 = -1, v1 = -1, v2 = -1, v3 = -1;
					int vt0 = -1, vt1 = -1, vt2 = -1, vt3 = -1;
					int vn0 = -1, vn1 = -1, vn2 = -1, vn3 = -1;

					// Reads a triangle or rectangle arguments IDs.

					// 3 or 4 vertices, positions, texture coords, normals
					int result = sscanf(currentLine.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &vt0, &vn0, &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
					if (result != 9 && result != 12)
					{
						v0 = -1, v1 = -1, v2 = -1, v3 = -1;
						vt0 = -1, vt1 = -1, vt2 = -1, vt3 = -1;
						vn0 = -1, vn1 = -1, vn2 = -1, vn3 = -1;

						// 3 or 4 vertices, positions, no texture coords, normals
						result = sscanf(currentLine.c_str(), "%*s %d//%d %d//%d %d//%d %d//%d", &v0, &vn0, &v1, &vn1, &v2, &vn2, &v3, &vn3);
						if (result != 6 && result != 8)
						{
							v0 = -1, v1 = -1, v2 = -1, v3 = -1;
							vt0 = -1, vt1 = -1, vt2 = -1, vt3 = -1;
							vn0 = -1, vn1 = -1, vn2 = -1, vn3 = -1;

							// 3 or 4 vertices, positions, texture coords, no normals
							result = sscanf(currentLine.c_str(), "%*s %d/%d/ %d/%d/ %d/%d/ %d/%d/", &v0, &vn0, &v1, &vn1, &v2, &vn2, &v3, &vn3);
							if (result != 6 && result != 8)
							{
								v0 = -1, v1 = -1, v2 = -1, v3 = -1;
								vt0 = -1, vt1 = -1, vt2 = -1, vt3 = -1;
								vn0 = -1, vn1 = -1, vn2 = -1, vn3 = -1;

								// 3 or 4 vertices, positions, no texture coords, no normals
								result = sscanf(currentLine.c_str(), "%*s %d// %d// %d// %d//", &v0, &vn0, &v1, &vn1, &v2, &vn2, &v3, &vn3);
								if (result != 3 && result != 4)
								{
									throw new std::invalid_argument("Unable to format OBJ faces.");
								}
							}
						}
					}

					objModel->AddVertexCoordTriangle(new ObjTriangle(v0, v1, v2));

					if (vt0 > 0)
					{
						objModel->AddTextureCoordTriangle(new ObjTriangle(vt0, vt1, vt2));
						objModel->SetHasTextureCoordinates(true);
					}

					if (vn0 > 0)
					{
						objModel->AddNormalTriangle(new ObjTriangle(vn0, vn1, vn2));
						objModel->SetHasNormals(true);
					}

					// If we read a rectangle, add the second triangle.
					if (v3 > 0)
					{
						objModel->AddVertexCoordTriangle(new ObjTriangle(v0, v2, v3));

						if (vt0 > 0)
						{
							objModel->AddTextureCoordTriangle(new ObjTriangle(vt0, vt2, vt3));
						}

						if (vn0 > 0)
						{
							objModel->AddNormalTriangle(new ObjTriangle(vn0, vn2, vn3));
						}
					}
				}
			}
		}

		objModel->ComputeFinalVerticesAndTriangles();

		return objModel;
	}
}
