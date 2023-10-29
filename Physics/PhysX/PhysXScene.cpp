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
