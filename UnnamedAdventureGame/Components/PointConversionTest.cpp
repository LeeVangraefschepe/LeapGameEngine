#include "PointConversionTest.h"

#include <SceneGraph/GameObject.h>
#include <Components/RenderComponents/CameraComponent.h>

#include <InputManager.h>
#include <Mouse.h>

#include <Components/Transform/Transform.h>
#include <Components/RenderComponents/MeshRenderer.h>
#include <Components/RenderComponents/UIComponents/CanvasComponent.h>
#include <Components/RenderComponents/UIComponents/CanvasActions.h>
#include <Components/RenderComponents/UIComponents/RectTransform.h>
#include <Components/RenderComponents/UIComponents/Image.h>
#include <Graphics/Mesh.h>

#include <Interfaces/ITexture.h>
#include <ServiceLocator/ServiceLocator.h>
#include <Interfaces/IRenderer.h>

#include <Graphics/Texture.h>

void unag::PointConversionTest::Start()
{
	m_FollowCursorGO = GetGameObject()->GetParent()->CreateChild("test");
	const leap::Mesh mesh{ "Data/Engine/Models/cube.obj", true };
	m_FollowCursorGO->AddComponent<leap::MeshRenderer>()->SetMesh(mesh);
	m_FollowCursorGO->GetTransform()->Scale(0.05f);

	auto pCanvasObj{ GetGameObject()->GetParent()->CreateChild("canvas") };
	pCanvasObj->AddComponent<leap::CanvasComponent>();
	pCanvasObj->AddComponent<leap::CanvasActions>();

	m_FollowCubeGO = pCanvasObj->CreateChild("uitest");
	const leap::Texture texture{ "Data/debug.png", true };
	m_FollowCubeGO->AddComponent<leap::RectTransform>()->SetSize(20.0f, 20.0f);
	m_FollowCubeGO->AddComponent<leap::Image>()->SetTexture(texture);
}

void unag::PointConversionTest::Update()
{
	auto camera{ GetGameObject()->GetComponent<leap::CameraComponent>() };
	m_FollowCursorGO->GetTransform()->SetWorldPosition(camera->ScreenToWorldSpace(leap::input::InputManager::GetInstance().GetMouse()->GetPos(), 0.9f));

	m_FollowCubeGO->GetTransform()->SetLocalPosition(glm::vec3{ camera->WorldToScreenSpace(m_FollowCursorGO->GetTransform()->GetWorldPosition()), 0.0f });
}
