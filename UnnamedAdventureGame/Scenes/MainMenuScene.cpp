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

#include <Interfaces/IAudioSystem.h>
#include <Interfaces/IAudioClip.h>

#include <Components/Audio/AudioSource.h>
#include <Components/Audio/AudioListener.h>

#include <iostream>
#include "../Components/AudioTester.h"

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pDirLight{ scene.CreateGameObject("Directional Light") };
	pDirLight->AddComponent<leap::DirectionalLightComponent>();
	pDirLight->GetTransform()->SetWorldRotation(60.0f, 0.0f, 0.0f);

	leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraObj->AddComponent<FreeCamMovement>();
	pCameraObj->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);
	pCameraObj->AddComponent<leap::AudioListener>();

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

	leap::audio::IAudioClip* pClip{ leap::ServiceLocator::GetAudio().LoadClip("Data/sound.mp3", true) };

	auto rotator{ scene.CreateGameObject("rotator") };
	rotator->AddComponent<Transformator>();

	auto bunnyMesh{ rotator->CreateChild("Bunny mesh") };
	leap::MeshRendererComponent* pBunnyMeshRenderer{ bunnyMesh->AddComponent<leap::MeshRendererComponent>() };
	pBunnyMeshRenderer->LoadMesh("Data/highpolybunnywithnormals.obj");
	pBunnyMeshRenderer->SetMaterial(pNormalMaterial);
	bunnyMesh->GetTransform()->Scale(10.0f);
	bunnyMesh->GetTransform()->Translate(-10.0f, 0.0f, 0.0f);
	bunnyMesh->AddComponent<Transformator>();
	leap::AudioSource* pAudio{ bunnyMesh->AddComponent<leap::AudioSource>() };
	pAudio->SetClip(pClip);
	pAudio->Set3DSound(true);
	pAudio->SetPlayOnAwake(true);
	pAudio->SetLooping(true);
	bunnyMesh->AddComponent<AudioTester>();

	const auto info{ scene.CreateGameObject("Info") };
	info->AddComponent<InfoUI>();

	const auto windowControls{ scene.CreateGameObject("Window") };
	windowControls->AddComponent<WindowManager>();
}
