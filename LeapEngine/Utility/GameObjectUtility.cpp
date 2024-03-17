#include "GameObjectUtility.h"

#include "../SceneGraph/GameObject.h"

#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"

#include "../Components/RenderComponents/MeshRenderer.h"

#include "../Components/Transform/Transform.h"

void leap::GameObjectUtility::CreateFromFBX(GameObject* pParent, const std::string& filePath)
{
	const leap::Mesh meshAndSubmeshes{ filePath };

	CreateFromFBXRecursive(pParent, meshAndSubmeshes);
}

void leap::GameObjectUtility::CreateFromFBXRecursive(GameObject* pParent, const Mesh& mesh)
{
	leap::GameObject* pMeshObject{ pParent->CreateChild(mesh.GetName().c_str()) };

	auto pTransform{ pMeshObject->GetTransform() };
	pTransform->SetLocalPosition(mesh.GetPosition());
	pTransform->SetLocalRotation(mesh.GetRotation());
	pTransform->SetLocalScale(mesh.GetScale());

	if (mesh.IsValid())
	{			
		auto pMeshRenderer{ pMeshObject->AddComponent<MeshRenderer>() };
		pMeshRenderer->SetMaterial(leap::Material{ "Default" });

		pMeshRenderer->SetMesh(mesh);
	}

	auto subMeshes{ mesh.GetSubMeshes() };
	if (!subMeshes.empty())
	{
		for(const Mesh& subMesh : subMeshes)
			CreateFromFBXRecursive(pMeshObject, subMesh);
	}
}
