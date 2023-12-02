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
#include <Components/Physics/CapsuleCollider.h>

#include <Interfaces/IPhysicsMaterial.h>
#include <Interfaces/IPhysics.h>

#include "../Components/ApplyForces.h"
#include "../Components/ColliderScaler.h"
#include "../Components/PrintVelocity.h"

#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Material.h>

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pDirLight{ scene.CreateGameObject("Directional Light") };
	pDirLight->AddComponent<leap::DirectionalLightComponent>();
	pDirLight->GetTransform()->SetWorldRotation(60.0f, 0.0f, 0.0f);

	leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pMainCamera->GetData()->SetFarPlane(1000.0f);
	pCameraObj->AddComponent<leap::AudioListener>();
	//pCameraObj->GetTransform()->SetLocalPosition(0.0f, 200.0f, -200.0f);
	pCameraObj->GetTransform()->SetLocalPosition(0.0f, 5.0f, -5.0f);
	pCameraObj->AddComponent<FreeCamMovement>();

	auto canvas{ scene.CreateGameObject("Canvas") };
	leap::CanvasComponent* pCanvas{ canvas->AddComponent<leap::CanvasComponent>() };
	pCanvas->SetReferenceResolution({ 1920,1080 });
	pCanvas->SetMatchMode(leap::CanvasComponent::MatchMode::MatchHeight);
	canvas->AddComponent<leap::CanvasActions>();

	const auto info{ scene.CreateGameObject("Info") };
	info->AddComponent<InfoUI>();

	const auto windowControls{ scene.CreateGameObject("Window") };
	windowControls->AddComponent<WindowManager>();

	auto pTerrain{ scene.CreateGameObject("Terrain") };
	pTerrain->AddComponent<leap::TerrainComponent>()->LoadHeightmap("Data/heightmap.png");

	auto pTerrain2{ scene.CreateGameObject("Terrain") };
	pTerrain2->AddComponent<leap::TerrainComponent>()->SetSize(1024);
	pTerrain2->AddComponent<SineWaveTerrain>();
	pTerrain2->GetTransform()->SetLocalPosition(-1024.0f, 0.0f, 0.0f);

	const leap::Mesh cube{ "Data/Engine/Models/cube.obj" };
	const leap::Mesh sphere{ "Data/Engine/Models/sphere.obj", true };

	const leap::Material cubeMat{ "Cube Material" };
	cubeMat.Set("gDiffuseMap", leap::Texture{ "Data/debug.png" });
	const leap::Material sphereMat{ leap::Material::Create<leap::graphics::shaders::PosNorm3D>("Sphere Material") };

	auto pCubeObj{ scene.CreateGameObject("Cube") };
	auto pCubeRenderer{ pCubeObj->AddComponent<leap::MeshRenderer>() };
	pCubeRenderer->SetMesh(cube);
	pCubeRenderer->SetMaterial(cubeMat);
	pCubeObj->GetTransform()->SetLocalPosition(-1.5f, 5.0f, 0.0f);

	auto pSphereObj{ scene.CreateGameObject("Sphere") };
	auto pSphereRenderer{ pSphereObj->AddComponent<leap::MeshRenderer>() };
	pSphereRenderer->SetMesh(sphere);
	pSphereRenderer->SetMaterial(sphereMat);
	pSphereObj->GetTransform()->SetLocalPosition(1.5f, 5.0f, 0.0f);
}
