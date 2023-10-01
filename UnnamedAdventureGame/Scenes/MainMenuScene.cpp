#include "MainMenuScene.h"

#include <iostream>

#include "SceneGraph/Scene.h"
#include "Components/RenderComponents/CameraComponent.h"
#include "Camera.h"

#include "InputManager.h"
#include "LambdaCommand.h"
#include "Components/Transform/Transform.h"

#include "ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

#include "GameContext/GameContext.h"

#include <Components/RenderComponents/MeshRenderer.h>
#include "../Components/Transformator.h"

#include "Shaders/Pos3D.h"
#include "Shaders/PosNorm3D.h"

#include "Interfaces/IMaterial.h"
#include "Components/RenderComponents/DirectionalLightComponent.h"
#include <Shaders/PosNormTex3D.h>

#include <Data/CustomMesh.h>

#include "Mouse.h"

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pDirLight{ scene.CreateGameObject("Directional Light") };
	pDirLight->AddComponent<leap::DirectionalLightComponent>();
	pDirLight->GetTransform()->SetWorldRotation(-0.577f, -0.577f, 0.577f);
	pDirLight->AddComponent<Transformator>();
	
	leap::GameObject* pCameraYawObj{ scene.CreateGameObject("Main Camera Yaw") };
	leap::GameObject* pCameraObj{ pCameraYawObj->CreateChild("Main Camera") };
	leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraYawObj->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);

	leap::GameObject* pCameraObj2{ scene.CreateGameObject("Other Camera") };
	leap::CameraComponent* pOtherCamera{ pCameraObj2->AddComponent<leap::CameraComponent>() };
	pOtherCamera->GetData()->SetColor(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	pCameraObj2->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);

	const auto pTexturedMaterial{ leap::ServiceLocator::GetRenderer().CreateMaterial(leap::graphics::shaders::PosNormTex3D::GetShader(), "Texture") };
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

	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]() 
			{ 
				leap::graphics::IRenderer& renderer{ leap::ServiceLocator::GetRenderer() };
				if (renderer.GetCamera() == pMainCamera->GetData())
				{
					pOtherCamera->SetAsActiveCamera(true);
				}
				else
				{
					pMainCamera->SetAsActiveCamera(true);
				}
			}),
		leap::input::InputManager::InputType::EventPress, 
		leap::input::InputManager::KeyboardInput::KeyB
	);

	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				static unsigned int antiAliasing{};
				leap::graphics::IRenderer& renderer{ leap::ServiceLocator::GetRenderer() };
				++antiAliasing %= 4;
				switch (antiAliasing)
				{
				case 0:
					renderer.SetAntiAliasing(leap::graphics::AntiAliasing::NONE);
					break;
				case 1:
					renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X2);
					break;
				case 2:
					renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X4);
					break;
				case 3:
					renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X8);
					break;
				}
			}),
		leap::input::InputManager::InputType::EventPress,
		leap::input::InputManager::KeyboardInput::KeyL
	);

	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				pCameraYawObj->GetTransform()->Translate(-pCameraObj->GetTransform()->GetRight() * 10.0f * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime());
			}),
		leap::input::InputManager::InputType::EventRepeat,
		leap::input::InputManager::KeyboardInput::KeyA
	);
	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				pCameraYawObj->GetTransform()->Translate(pCameraObj->GetTransform()->GetRight() * 10.0f * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime());
			}),
		leap::input::InputManager::InputType::EventRepeat,
		leap::input::InputManager::KeyboardInput::KeyD
	);
	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				pCameraYawObj->GetTransform()->Translate(pCameraObj->GetTransform()->GetForward() * 10.0f * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime());
			}),
		leap::input::InputManager::InputType::EventRepeat,
		leap::input::InputManager::KeyboardInput::KeyW
	);
	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				pCameraYawObj->GetTransform()->Translate(-pCameraObj->GetTransform()->GetForward() * 10.0f * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime());
			}),
		leap::input::InputManager::InputType::EventRepeat,
		leap::input::InputManager::KeyboardInput::KeyS
	);
	leap::input::InputManager::GetInstance().GetMouse()->AddCommand(
		std::make_shared<leap::LambdaCommand>([=]()
			{
				auto mouseDelta = static_cast<glm::vec2>(leap::input::InputManager::GetInstance().GetMouse()->GetDelta());
				const auto deltaTime = leap::GameContext::GetInstance().GetTimer()->GetDeltaTime();
				constexpr float mouseSpeed = 15.f;

				pCameraYawObj->GetTransform()->Rotate(0.0f, mouseDelta.x * deltaTime * mouseSpeed, 0.0f);
				pCameraObj->GetTransform()->Rotate(mouseDelta.y * deltaTime * mouseSpeed, 0.0f, 0.0f);
			}),
		leap::input::InputManager::InputType::EventRepeat,
		leap::input::Mouse::Button::LeftButton
	);
}
