#include "MainMenuScene.h"

#include "SceneGraph/Scene.h"
#include "Components/RenderComponents/CameraComponent.h"
#include "Camera.h"

#include "Components/Transform/Transform.h"

#include "ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

#include "GameContext/GameContext.h"
#include "GameContext/Window.h"

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

#include "Debug.h"
#include <Interfaces/IAudioSystem.h>
#include <Interfaces/IAudioClip.h>

#include <Components/Audio/AudioSource.h>
#include <Components/Audio/AudioListener.h>

#include "../Components/AudioTester.h"

#include "Shaders/Heightmap.h"
#include <Components/RenderComponents/TerrainComponent.h>

#include <Data/Vertex.h>
#include "../Components/SineWaveTerrain.h"

#include <Components/Physics/BoxCollider.h>
#include <Components/Physics/Rigidbody.h>
#include <Components/Physics/SphereCollider.h>

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pDirLight{ scene.CreateGameObject("Directional Light") };
	pDirLight->AddComponent<leap::DirectionalLightComponent>();
	pDirLight->GetTransform()->SetWorldRotation(60.0f, 0.0f, 0.0f);

	leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pMainCamera->GetData()->SetFarPlane(1000.0f);
	pCameraObj->AddComponent<FreeCamMovement>();
	pCameraObj->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);
	pCameraObj->AddComponent<leap::AudioListener>();

	auto canvas{ scene.CreateGameObject("Canvas") };
	leap::CanvasComponent* pCanvas{ canvas->AddComponent<leap::CanvasComponent>() };
	pCanvas->SetReferenceResolution({ 1920,1080 });
	pCanvas->SetMatchMode(leap::CanvasComponent::MatchMode::MatchHeight);
	canvas->AddComponent<leap::CanvasActions>();

	const auto pTexturedMaterial{ leap::ServiceLocator::GetRenderer().CloneMaterial("Default", "Texture") };
	pTexturedMaterial->SetTexture("gDiffuseMap", leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));

	auto shadedMesh{ scene.CreateGameObject("Mesh") };
	leap::MeshRendererComponent* pShadedMeshRenderer{ shadedMesh->AddComponent<leap::MeshRendererComponent>() };
	pShadedMeshRenderer->LoadMesh("Data/plane.obj");
	pShadedMeshRenderer->SetMaterial(pTexturedMaterial);
	shadedMesh->GetTransform()->Scale(10.0f);
	shadedMesh->GetTransform()->SetLocalPosition(0, -1.0f, 0);

	const auto info{ scene.CreateGameObject("Info") };
	info->AddComponent<InfoUI>();

	const auto windowControls{ scene.CreateGameObject("Window") };
	windowControls->AddComponent<WindowManager>();

	auto pBox{ scene.CreateGameObject("Box") };
	pBox->AddComponent<leap::BoxCollider>();
	pBox->AddComponent<leap::Rigidbody>()->SetVelocity({ 0.0f, 10.0f, 0.0f});
	pBox->AddComponent<leap::MeshRendererComponent>()->LoadMesh("Data/Engine/Models/cube.obj");
	pBox->GetTransform()->Translate(0.0f, 0.0f, 0.0f);
	pBox->GetTransform()->Rotate(45.0f, 45.0f, 80.0f);

	auto pBox2{ scene.CreateGameObject("Box2") };
	pBox2->AddComponent<leap::BoxCollider>();
	pBox2->AddComponent<leap::MeshRendererComponent>()->LoadMesh("Data/Engine/Models/cube.obj");
	pBox2->GetTransform()->Translate(0.0f, -1.5f, 0.0f);
}
