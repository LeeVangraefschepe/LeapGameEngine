#include "PhysXScene.h"

#include "PxPhysicsAPI.h"

#include "PhysXEngine.h"

leap::physics::PhysXScene::PhysXScene(physx::PxScene* pScene)
	: m_pScene{ pScene }
{
}

leap::physics::PhysXScene::~PhysXScene()
{
	m_pScene->release();
}

void leap::physics::PhysXScene::Simulate(float fixedDeltaTime)
{
	m_pScene->simulate(fixedDeltaTime);
	m_pScene->fetchResults(true);
}

void leap::physics::PhysXScene::AddActor(physx::PxRigidActor* pActor)
{
	m_pScene->addActor(*pActor);
}

void leap::physics::PhysXScene::RemoveActor(physx::PxRigidActor* pActor)
{
	m_pScene->removeActor(*pActor);
}
