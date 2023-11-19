#include "PhysXScene.h"

#include "PxPhysicsAPI.h"

#include "PhysXEngine.h"

#include "../Data/RaycastHit.h"

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

void leap::physics::PhysXScene::SetEnabledDebugDrawing(bool isEnabled)
{
	m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, isEnabled ? 1.0f : 0.0f);
	m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, isEnabled ? 1.0f : 0.0f);
}

std::vector<std::pair<glm::vec3, glm::vec3>> leap::physics::PhysXScene::GetDebugDrawings()
{
	std::vector<std::pair<glm::vec3, glm::vec3>> debugDrawings{};

	const physx::PxRenderBuffer& rb = m_pScene->getRenderBuffer();
	const auto pLines{ rb.getLines() };
	for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
	{
		const physx::PxDebugLine& line = pLines[i];
		debugDrawings.emplace_back(glm::vec3{ line.pos0.x, line.pos0.y, line.pos0.z },
								   glm::vec3{ line.pos1.x, line.pos1.y, line.pos1.z });
	}

	return debugDrawings;
}

bool leap::physics::PhysXScene::Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHit& hitInfo)
{
	physx::PxRaycastBuffer physXRaycastHit{};
	physx::PxVec3 physXStart{ start.x, start.y, start.z};
	physx::PxVec3 physXDir{ direction.x, direction.y, direction.z };

	if (!m_pScene->raycast(physXStart, physXDir.getNormalized(), distance, physXRaycastHit)) return { false };

	hitInfo.pCollider = physXRaycastHit.block.shape->userData;
	hitInfo.distance = physXRaycastHit.block.distance;
	const auto& physXPoint{ physXRaycastHit.block.position };
	hitInfo.point = { physXPoint.x, physXPoint.y, physXPoint.z };
	const auto& physXNormal{ physXRaycastHit.block.normal };
	hitInfo.normal = { physXNormal.x, physXNormal.y, physXNormal.z };

	return true;
}

void leap::physics::PhysXScene::AddActor(physx::PxRigidActor* pActor) const
{
	m_pScene->addActor(*pActor);
}

void leap::physics::PhysXScene::RemoveActor(physx::PxRigidActor* pActor) const
{
	m_pScene->removeActor(*pActor);
}
