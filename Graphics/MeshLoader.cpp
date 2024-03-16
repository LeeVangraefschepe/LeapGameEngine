#include "MeshLoader.h"

#include <fbxsdk.h>

bool leap::graphics::MeshLoader::ParseFbx(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	std::vector<glm::vec3> positions{};
	std::vector<glm::vec3> normals{};
	std::vector<glm::vec2> UVs{};

	vertices.clear();
	indices.clear();

	static auto pManager = FbxManager::Create();

	FbxString fullFbxFile{ filePath.c_str() };
	if (!FbxFileUtils::Exist(fullFbxFile))
	{
		return false;
	}

	//create an importer
	FbxImporter* importer = FbxImporter::Create(pManager, "");

	auto fileName{ filePath.substr(filePath.rfind('/') + 1) };

	//initialize the importer by providing a file name
	const bool imorterStatus = importer->Initialize(filePath.c_str(), -1, pManager->GetIOSettings());
	if (!imorterStatus)
	{
		FBXSDK_printf("error: initialize importer failed\n");
		return false;
	}

	if (!importer->IsFBX()) return false;

	//import the scene
	static fbxsdk::FbxScene* pScene = FbxScene::Create(pManager, "fbxscene");
	if (!pScene) {
		FBXSDK_printf("error: unable to create FBX scene\n");
		return false;
	}

	if (!importer->Import(pScene)) return false;

	//convert axis system to what is used in this example, if needed
	FbxAxisSystem sceneAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem localAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (sceneAxisSystem != localAxisSystem)
	{
		localAxisSystem.ConvertScene(pScene);
	}

	LoadFbxRecursive(pScene->GetRootNode(), vertices, indices);

	return true;
}

void leap::graphics::MeshLoader::LoadFbxRecursive(fbxsdk::FbxNode* pNode, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = pNode->GetMesh();

			for (int i = 0; i < lMesh->GetPolygonCount(); ++i) 
			{
				for (int j = 0; j < 3; ++j) 
				{
					fbxsdk::FbxVector4 normal;
					lMesh->GetPolygonVertexNormal(i, j, normal);

					int controlPointIndex = lMesh->GetPolygonVertex(i, j);
					auto position = lMesh->GetControlPointAt(controlPointIndex);

					Vertex v{};
					v.position = glm::vec3{ position[0], position[1], position[2] };
					v.normal = glm::vec3{ normal[0], normal[1], normal[2] };

					vertices.emplace_back(v);
					indices.emplace_back((int)vertices.size() - 1);
				}
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadFbxRecursive(pNode->GetChild(lChildIndex), vertices, indices);
	}
}

