#include "MeshLoadingTest.h"

#include <SceneGraph/Scene.h>
#include <SceneGraph/GameObject.h>

#include <Components/RenderComponents/CameraComponent.h>
#include <Components/RenderComponents/MeshRenderer.h>

#include <Components/Transform/Transform.h>

#include "../Components/FreeCamMovement.h"

#include <Graphics/Mesh.h>
#include <Graphics/Material.h>

#include <Shaders/PosNorm3D.h>

void unag::MeshLoadingTest::Load(leap::Scene& scene)
{
	auto camObj{ scene.CreateGameObject("Camera") };
	camObj->AddComponent<leap::CameraComponent>()->SetAsActiveCamera(true);
	camObj->AddComponent<FreeCamMovement>();

	const leap::Mesh fbxCube{ "Data/cube.fbx", true };
	const leap::Mesh objCube{ "Data/Engine/Models/cube.obj", true };
	const leap::Material material{ leap::Material::Create<leap::graphics::shaders::PosNorm3D>("diorama") };

	auto fbxMeshObj{ scene.CreateGameObject("FBX Mesh") };
	auto fbxMeshRenderer{ fbxMeshObj->AddComponent<leap::MeshRenderer>() };
	fbxMeshRenderer->SetMesh(fbxCube);
	fbxMeshRenderer->SetMaterial(material);

	auto objMeshObj{ scene.CreateGameObject("OBJ Mesh") };
	auto objMeshRenderer{ objMeshObj->AddComponent<leap::MeshRenderer>() };
	objMeshRenderer->SetMesh(objCube);
	objMeshRenderer->SetMaterial(material);

	fbxMeshObj->GetTransform()->SetLocalPosition(2.0f, 0.0f, 5.0f);
	objMeshObj->GetTransform()->SetLocalPosition(-2.0f, 0.0f, 5.0f);
}