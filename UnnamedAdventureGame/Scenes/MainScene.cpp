#include "MainScene.h"

#include <SceneGraph/SceneManager.h>
#include <SceneGraph/Scene.h>
#include <SceneGraph/GameObject.h>

#include <Components/RenderComponents/CameraComponent.h>

#include "../Components/ImGuiSceneChooser.h"

void unag::MainScene::Load(leap::Scene& scene)
{
	leap::GameObject* pCameraObj{ scene.CreateGameObject("Main Camera") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraObj->AddComponent<ImGuiSceneChooser>();
}
