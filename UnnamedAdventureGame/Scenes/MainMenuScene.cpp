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

#include "../Components/PrintCollision.h"
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

	/*auto sprite{ canvas->CreateChild("Sprite") };
	sprite->AddComponent<leap::RectTransform>();
	sprite->AddComponent<leap::Image>()->SetTexture(leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png"));*/

	const auto info{ scene.CreateGameObject("Info") };
	info->AddComponent<InfoUI>();

	const auto windowControls{ scene.CreateGameObject("Window") };
	windowControls->AddComponent<WindowManager>();

	//const leap::Material material{ "Test", leap::Shader<leap::graphics::shaders::PosNorm3D>{} };

	//const auto pTexturedMaterial{ leap::ServiceLocator::GetRenderer().CloneMaterial("Default", "Texture") };
	//pTexturedMaterial->SetTexture("gDiffuseMap", leap::ServiceLocator::GetRenderer().CreateTexture("Data/debug.png", true));

	/*const leap::Mesh cube{ "Data/Engine/Models/cube.obj" };
	auto ground{ scene.CreateGameObject("Ground") };
	leap::MeshRenderer* pGroundMeshRenderer{ ground->AddComponent<leap::MeshRenderer>() };
	pGroundMeshRenderer->SetMesh(cube);
	pGroundMeshRenderer->SetMaterial(material);
	ground->GetTransform()->SetLocalPosition(0.0f, 5.0f, 0.0f);*/

	//auto terrain{ scene.CreateGameObject("Terrain") };
	//leap::TerrainComponent* pTerrain{ terrain->AddComponent<leap::TerrainComponent>() };

	////pTerrain->SetSize(1000);
	//pTerrain->GetTexture().Load("Data/Heightmap.png");
	////terrain->AddComponent<SineWaveTerrain>();
	//auto heights = pTerrain->GetHeights();
	//for (int x{}; x < 1024; ++x)
	//{
	//	float xMultiplier = x / 1024.0f;
	//	for (int y{}; y < 1024; ++y)
	//	{
	//		float yMultiplier = y / 1024.0f;

	//		heights[x + y * 1024] += xMultiplier * yMultiplier;
	//		if (heights[x + y * 1024] > 1.0f) heights[x + y * 1024] = 1.0f;
	//	}
	//}
	//pTerrain->SetHeights(heights);
	
	//pTerrain->GetTexture().Load("Data/Heightmap.png");
	//auto test{ pTerrain->GetHeights() };
	//for (auto& var : test)
	//{
	//	if (var < 0.35f) var = 0.35f;
	//}
	//pTerrain->SetHeights(test);


	// 
	//auto heights{ pTerrain->GetHeights() };

	//heights[2] = 0.1f;
	///*for (int x{5}; x < 15; ++x)
	//{
	//	for (int z{5}; z < 15; ++z)
	//	{
	//		heights[x + z * 1000] = 0.1f;
	//	}
	//}*/
	//pTerrain->SetHeights(heights);

	//pGroundMeshRenderer->SetMaterial(pTexturedMaterial);
	/*ground->GetTransform()->SetLocalPosition(0.0f, -1.0f, 0.0f);
	ground->GetTransform()->SetLocalScale(20.0f, 1.0f, 20.0f);
	ground->AddComponent<leap::BoxCollider>();*/


	/*auto box{ scene.CreateGameObject("Box") };
	leap::MeshRenderer* pBoxMeshRenderer{ box->AddComponent<leap::MeshRenderer>() };
	pBoxMeshRenderer->SetMesh(cube);
	pBoxMeshRenderer->SetMaterial(pTexturedMaterial);
	box->GetTransform()->SetLocalPosition(0.0f, 0.5f, 0.0f);
	box->AddComponent<leap::BoxCollider>();
	box->AddComponent<leap::Rigidbody>();*/


	auto terrain{ scene.CreateGameObject("Terrain") };
	terrain->AddComponent<leap::TerrainComponent>()->SetSize(1024);
	terrain->AddComponent<SineWaveTerrain>();

	auto terrain2{ scene.CreateGameObject("Terrain") };
	terrain2->AddComponent<leap::TerrainComponent>()->SetSize(1024);
	terrain2->AddComponent<SineWaveTerrain>();
	terrain2->GetTransform()->SetLocalPosition(-1024.0f, 0.0f, 0.0f);

	auto terrain3{ scene.CreateGameObject("Terrain") };
	terrain3->AddComponent<leap::TerrainComponent>()->SetSize(1024);
	terrain3->AddComponent<SineWaveTerrain>();
	terrain3->GetTransform()->SetLocalPosition(0.0f, 0.0f, 1024.0f);

	auto terrain4{ scene.CreateGameObject("Terrain") };
	terrain4->AddComponent<leap::TerrainComponent>()->SetSize(1024);
	terrain4->AddComponent<SineWaveTerrain>();
	terrain4->GetTransform()->SetLocalPosition(-1024.0f, 0.0f, 1024.0f);
}
