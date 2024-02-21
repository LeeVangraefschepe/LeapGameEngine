#include "ParticleTest.h"

#include <SceneGraph/Scene.h>
#include <SceneGraph/GameObject.h>

#include <Components/RenderComponents/CameraComponent.h>
#include <Components/RenderComponents/ParticleRenderer.h>
#include <Components/Transform/Transform.h>

#include <Graphics/Texture.h>

#include "../Components/FreeCamMovement.h"
#include "../Components/Transformator.h"

void unag::ParticleTest::Load(leap::Scene& scene)
{
	leap::GameObject* pCameraObj{ scene.CreateGameObject("Extended Camera Test") };
	const leap::CameraComponent* pMainCamera{ pCameraObj->AddComponent<leap::CameraComponent>() };
	pMainCamera->SetAsActiveCamera(true);
	pCameraObj->GetTransform()->SetWorldPosition(0.0f, 0.0f, -5.0f);
	pCameraObj->AddComponent<FreeCamMovement>();

	leap::GameObject* pSmokeRoot{ scene.CreateGameObject("Smoke root") };
	pSmokeRoot->AddComponent<Transformator>();

	leap::GameObject* pSmokeObj{ pSmokeRoot->CreateChild("Smoke Particle") };
	pSmokeObj->GetTransform()->SetLocalPosition(5.0f, 0.0f, 0.0f);
	leap::ParticleRenderer* pSmoke{ pSmokeObj->AddComponent<leap::ParticleRenderer>() };
	pSmoke->SetTexture(leap::Texture{ "Data/Smoke.png" });
	pSmoke->GetEmitterSettings().SetVelocity(0.0f, 6.0f, 0.0f);
	pSmoke->GetEmitterSettings().SetSize(2.0f);
	pSmoke->GetEmitterSettings().SetScalePerSecond(1.5f);
	pSmoke->GetEmitterSettings().SetSpawnRadius(0.1f);
	pSmoke->GetEmitterSettings().SetEndVelocity(0.0f, 1.0f, 0.0f);
	pSmoke->GetEmitterSettings().maxRotation = 2.0f * glm::pi<float>();
	pSmoke->GetEmitterSettings().particlesPerSecond = 20;
	pSmoke->GetEmitterSettings().maxParticles = 200;

	leap::GameObject* pFireObj{ scene.CreateGameObject("Fire Particle") };
	leap::ParticleRenderer* pFire{ pFireObj->AddComponent<leap::ParticleRenderer>() };
	pFire->SetTexture(leap::Texture{ "Data/FireBall.png" });
	pFire->GetEmitterSettings().SetVelocity(0.0f, 1.5f, 0.0f);
	pFire->GetEmitterSettings().SetSize(2.5f);
	pFire->GetEmitterSettings().SetScalePerSecond(1.5f);
	pFire->GetEmitterSettings().SetSpawnRadius(0.1f);
	pFire->GetEmitterSettings().SetEndVelocity(0.0f, 1.0f, 0.0f);
	pFire->GetEmitterSettings().minRotation = -glm::half_pi<float>();
	pFire->GetEmitterSettings().maxRotation = glm::half_pi<float>();
	pFire->GetEmitterSettings().particlesPerSecond = 20;
	pFire->GetEmitterSettings().maxParticles = 200;
	pFire->GetEmitterSettings().SetLifetime(2.5f);
	pFire->GetEmitterSettings().particlesPerSecond = 4;
	pFire->GetEmitterSettings().endColor = glm::vec4{ 0.2f, 0.0f, 0.0f, 0.0f };
	pFireObj->GetTransform()->SetLocalPosition(2.0f, 0.0f, 0.0f);
}
