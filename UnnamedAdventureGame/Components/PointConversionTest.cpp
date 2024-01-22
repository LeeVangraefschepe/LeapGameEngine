#include "PointConversionTest.h"

#include <SceneGraph/GameObject.h>
#include <Components/RenderComponents/CameraComponent.h>

#include <InputManager.h>
#include <Mouse.h>

#include <Components/Transform/Transform.h>
#include <Components/RenderComponents/MeshRenderer.h>
#include <Graphics/Mesh.h>

void unag::PointConversionTest::Start()
{
	m_GO = GetGameObject()->GetParent()->CreateChild("test");
	const leap::Mesh mesh{ "Data/Engine/Models/cube.obj", true };
	m_GO->AddComponent<leap::MeshRenderer>()->SetMesh(mesh);
	m_GO->GetTransform()->Scale(0.05f);
}

void unag::PointConversionTest::Update()
{
	auto camera{ GetGameObject()->GetComponent<leap::CameraComponent>() };
	m_GO->GetTransform()->SetWorldPosition(camera->ScreenToWorldSpace(leap::input::InputManager::GetInstance().GetMouse()->GetPos(), 0.9f));
}
