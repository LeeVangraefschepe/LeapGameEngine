#include "MeshLoader.h"

#include <fbxsdk.h>

#include <Quaternion.h>

bool leap::graphics::MeshLoader::ParseFbx(const std::string& filePath, MeshData& meshData)
{
	std::vector<glm::vec3> positions{};
	std::vector<glm::vec3> normals{};
	std::vector<glm::vec2> UVs{};

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

	LoadFbxRecursive(pScene->GetRootNode(), meshData);

	return true;
}

void leap::graphics::MeshLoader::LoadFbxRecursive(fbxsdk::FbxNode* pNode, MeshData& meshData)
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if (pNodeAttribute)
	{
		if (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();

			std::vector<Vertex> vertices{};
			std::vector<unsigned int> indices{};

			for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
			{
				for (int j = 0; j < 3; ++j) 
				{
					fbxsdk::FbxVector4 normal;
					pMesh->GetPolygonVertexNormal(i, j, normal);

					int controlPointIndex = pMesh->GetPolygonVertex(i, j);
					auto position = pMesh->GetControlPointAt(controlPointIndex);

					Vertex v{};
					v.position = glm::vec3{ position[0], position[1], position[2] };
					v.normal = glm::vec3{ normal[0], normal[1], normal[2] };
					

					vertices.emplace_back(v);
					indices.emplace_back((int)vertices.size() - 1);
				}
			}

			const auto& globalTransform{ pNode->EvaluateLocalTransform() };
			const auto& scale{ globalTransform.GetS() };
			const auto& rotation{ globalTransform.GetR() };
			const auto& position{ globalTransform.GetT() };

			meshData.position = glm::vec3{ position[0], position[1], position[2] };
			meshData.rotation = Quaternion::FromEuler(static_cast<float>(rotation[0]), static_cast<float>(rotation[1]), static_cast<float>(rotation[2]));
			meshData.scale = glm::vec3{ scale[0], scale[1], scale[2] };

			meshData.vertexBuffers.emplace_back(std::move(vertices));
			meshData.indexBuffers.emplace_back(std::move(indices));
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		MeshData childMeshData{};
		LoadFbxRecursive(pNode->GetChild(lChildIndex), childMeshData);
		meshData.subMeshes.emplace_back(childMeshData);
	}
}

