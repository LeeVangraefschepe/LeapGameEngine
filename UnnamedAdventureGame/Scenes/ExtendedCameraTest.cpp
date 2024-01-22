#include "ExtendedCameraTest.h"

#include <SceneGraph/Scene.h>
#include <SceneGraph/GameObject.h>

#include <Components/RenderComponents/CameraComponent.h>
#include <Components/Transform/Transform.h>

#include "../Components/PointConversionTest.h"

void unag::ExtendedCameraTest::Load(leap::Scene& scene)
{
	leap::GameObject* pCameraObj{ scene.CreateGameObject("Extended Camera Test") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraObj->AddComponent<PointConversionTest>();
}
