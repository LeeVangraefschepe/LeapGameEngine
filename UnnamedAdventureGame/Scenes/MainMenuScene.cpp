#include "MainMenuScene.h"
#include "SceneGraph/Scene.h"
#include "Components/RenderComponents/CameraComponent.h"
#include "Camera.h"

#include "InputManager.h"
#include "LambdaCommand.h"

#include "ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetActive(true);

	leap::GameObject* pCameraObj2{ scene.CreateGameObject("Other Camera") };
	leap::CameraComponent* pOtherCamera{ pCameraObj2->AddComponent<leap::CameraComponent>() };
	pOtherCamera->GetData()->SetColor(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	leap::input::InputManager::GetInstance().AddCommand(
		std::make_shared<leap::LambdaCommand>([=]() 
			{ 
				leap::graphics::IRenderer& renderer{ leap::ServiceLocator::GetRenderer() };
				if (renderer.GetCamera() == pMainCamera->GetData())
				{
					pOtherCamera->SetActive(true);
				}
				else
				{
					pMainCamera->SetActive(true);
				}
			}),
		leap::input::InputManager::InputType::EventPress, 
		leap::input::InputManager::KeyboardInput::KeyB
	);
}