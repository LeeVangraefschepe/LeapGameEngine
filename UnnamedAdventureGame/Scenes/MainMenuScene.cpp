#include "MainMenuScene.h"

#include "SceneGraph/Scene.h"
#include "Components/RenderComponents/CameraComponent.h"
#include "Camera.h"

#include "Components/Transform/Transform.h"

#include "ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

#include "GameContext/GameContext.h"

#include <Components/RenderComponents/MeshRenderer.h>
#include "../Components/Transformator.h"
#include "../Components/InfoUI.h"

#include "Shaders/Pos3D.h"
#include "Shaders/PosNorm3D.h"

#include "Interfaces/IMaterial.h"
#include "Components/RenderComponents/DirectionalLightComponent.h"
#include <Shaders/PosNormTex3D.h>

#include <Data/CustomMesh.h>

#include "Components/RenderComponents/UIComponents/Image.h"
#include "../Components/FreeCamMovement.h"
#include "../Components/WindowManager.h"
#include "Components/RenderComponents/UIComponents/CanvasComponent.h"
#include "Components/RenderComponents/UIComponents/RectTransform.h"
#include "Components/RenderComponents/UIComponents/Button.h"
#include "Components/RenderComponents/UIComponents/CanvasActions.h"

#include <iostream>

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pDirLight{ scene.CreateGameObject("Directional Light") };
	pDirLight->AddComponent<leap::DirectionalLightComponent>();
	pDirLight->GetTransform()->SetWorldRotation(60.0f, 0.0f, 0.0f);

	/*leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraObj->AddComponent<FreeCamMovement>();
	pCameraObj->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);*/

	const auto pTexturedMaterial{ leap::ServiceLocator::GetRenderer().CloneMaterial("Default", "Texture") };
	pTexturedMaterial->SetTexture("gDiffuseMap", leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));

	const auto pNormalMaterial{ leap::ServiceLocator::GetRenderer().CreateMaterial(leap::graphics::shaders::PosNorm3D::GetShader(), "Normals") };

	auto shadedMesh{ scene.CreateGameObject("Mesh") };
	leap::MeshRendererComponent* pShadedMeshRenderer{ shadedMesh->AddComponent<leap::MeshRendererComponent>() };
	pShadedMeshRenderer->LoadMesh("Data/plane.obj");
	pShadedMeshRenderer->SetMaterial(pTexturedMaterial);
	shadedMesh->GetTransform()->Scale(10.0f);
	shadedMesh->GetTransform()->SetLocalPosition(0, -1.0f, 0);

	auto customMesh{ scene.CreateGameObject("Custom mesh") };
	leap::MeshRendererComponent* pCustomMeshRenderer{ customMesh->AddComponent<leap::MeshRendererComponent>() };

	leap::graphics::CustomMesh customMeshData{};
	customMeshData.vertices =
	{
		leap::graphics::Vertex{ { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		leap::graphics::Vertex{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		leap::graphics::Vertex{ { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } }
	};
	customMeshData.indices =
	{
		0,1,2
	};
	pCustomMeshRenderer->LoadMesh(customMeshData);
	pCustomMeshRenderer->SetMaterial(pNormalMaterial);

	auto canvas{ scene.CreateGameObject("Canvas") };
	leap::CanvasComponent* pCanvas{ canvas->AddComponent<leap::CanvasComponent>() };
	pCanvas->SetReferenceResolution({ 1920,1080 });
	pCanvas->SetMatchMode(leap::CanvasComponent::MatchMode::MatchHeight);
	canvas->AddComponent<leap::CanvasActions>();

	const glm::vec2 screenSize{ leap::GameContext::GetInstance().GetWindow()->GetWindowSize() };

	auto fssprite{ canvas->CreateChild("Sprite") };
	leap::RectTransform* pRT{ fssprite->AddComponent<leap::RectTransform>() };
	leap::Image* pFS{ fssprite->AddComponent<leap::Image>() };
	pFS->SetTexture(leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));
	pRT->SetSize(1920.0f / 2, 1080);
	pRT->SetReferencePosition(480, 0);
	pRT->SetDepth(2);

	auto sprite2{ fssprite->CreateChild("Sprite") };
	leap::RectTransform* pRT2{ sprite2->AddComponent<leap::RectTransform>() };
	leap::Image* pImage2{ sprite2->AddComponent<leap::Image>() };
	pImage2->SetTexture(leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));
	pImage2->SetPivot(1.0f, 1.0f);
	pImage2->SetNativeSize();
	pRT2->SetLocalReferencePosition(-480, 0);

	auto sprite{ sprite2->CreateChild("Sprite") };
	leap::RectTransform* pRT3{ sprite->AddComponent<leap::RectTransform>() };
	leap::Image* pImage{ sprite->AddComponent<leap::Image>() };
	pImage->SetTexture(leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));
	pImage->SetPivot(0.0f, 0.0f);
	pRT3->SetSize(100, 100);
	pRT3->SetReferencePosition(0, 100);
	sprite->AddComponent<leap::Button>()->OnClicked.AddListener([](const leap::Button&) { std::cout << "Button click\n"; });
	pRT3->SetDepth(5);

	auto bunnyMesh{ scene.CreateGameObject("Bunny mesh") };
	leap::MeshRendererComponent* pBunnyMeshRenderer{ bunnyMesh->AddComponent<leap::MeshRendererComponent>() };
	pBunnyMeshRenderer->LoadMesh("Data/highpolybunnywithnormals.obj");
	pBunnyMeshRenderer->SetMaterial(pNormalMaterial);
	bunnyMesh->GetTransform()->Scale(10.0f);
	bunnyMesh->GetTransform()->Translate(-5.0f, 0.0f, 0.0f);
	bunnyMesh->AddComponent<Transformator>();

	const auto info{ scene.CreateGameObject("Info") };
	info->AddComponent<InfoUI>();

	const auto windowControls{ scene.CreateGameObject("Window") };
	windowControls->AddComponent<WindowManager>();
}
