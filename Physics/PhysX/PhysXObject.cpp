#include "PhysXObject.h"

#include "PhysXShapes.h"
#include "PhysXEngine.h"
#include "PhysXScene.h"

#include <PxPhysics.h>
#include <PxRigidActor.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxRigidActor.h>
#include <PxScene.h>

leap::physics::PhysXObject::PhysXObject(void* pOwner)
	: m_pOwner{ pOwner }
{
}

leap::physics::PhysXObject::~PhysXObject()
{
	m_pActor->release();
}

void leap::physics::PhysXObject::Update(PhysXEngine* pEngine, IPhysicsScene* pScene)
{
	if (m_IsObjectDirty) UpdateObject(pEngine, pScene);
	if (m_IsTransformDirty) UpdateTransform();
	if (m_pRigidbody && m_pRigidbody->IsDirty()) UpdateRigidbody();

	if (!IsValid()) static_cast<PhysXScene*>(pScene)->RemoveActor(m_pActor);
}

void leap::physics::PhysXObject::Apply(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& setFunc, const std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)> getFunc)
{
	if (m_pRigidbody == nullptr)
	{
		auto transformPair{ getFunc(m_pOwner) };

		const physx::PxVec3 position{ transformPair.first.x, transformPair.first.y, transformPair.first.z };
		const physx::PxQuat rotation{ transformPair.second.x, transformPair.second.y, transformPair.second.z, transformPair.second.w };

		m_pActor->setGlobalPose(physx::PxTransform{ position, rotation });

		return;
	}

	const physx::PxTransform transform{ m_pActor->getGlobalPose() };

	const glm::vec3 position{ transform.p.x, transform.p.y, transform.p.z };
	const glm::quat rotation{ transform.q.w, transform.q.x, transform.q.y, transform.q.z };

	setFunc(m_pOwner, position, rotation);
}

void leap::physics::PhysXObject::AddShape(IShape* pShape)
{
	IPhysXShape* pPhysXShape{ reinterpret_cast<IPhysXShape*>(pShape) };
	m_pShapes.emplace_back(pPhysXShape);

	if (m_pActor) m_pActor->attachShape(pPhysXShape->GetShape());
}

void leap::physics::PhysXObject::RemoveShape(IShape* pShape)
{
	IPhysXShape* pPhysXShape{ reinterpret_cast<IPhysXShape*>(pShape) };

	if (m_pActor)
	{
		m_pActor->detachShape(pPhysXShape->GetShape());
	}

	m_pShapes.erase(std::remove(begin(m_pShapes), end(m_pShapes), pPhysXShape));
}

leap::physics::IPhysicsObject::Rigidbody* leap::physics::PhysXObject::SetRigidbody(bool hasRigidbody)
{
	if (!hasRigidbody)
	{
		m_pRigidbody = nullptr;
		return nullptr;
	}

	if (m_pRigidbody == nullptr) m_pRigidbody = std::make_unique<Rigidbody>();

	return m_pRigidbody.get();
}

bool leap::physics::PhysXObject::IsValid()
{
	return m_pRigidbody != nullptr || !m_pShapes.empty();
}

void leap::physics::PhysXObject::SetTransform(const glm::vec3& position, const glm::quat& rotation)
{
	m_Position = position;
	m_Rotation = rotation;
	m_IsTransformDirty = true;
}

void leap::physics::PhysXObject::UpdateObject(PhysXEngine* pEngine, IPhysicsScene* pScene)
{
	m_IsObjectDirty = false;

	PhysXScene* pPhysXScene{ static_cast<PhysXScene*>(pScene) };

	if (m_pActor)
	{
		pPhysXScene->RemoveActor(m_pActor);
		m_pActor->release();
	}

	if (m_pRigidbody)
	{
		m_pActor = pEngine->GetPhysics()->createRigidDynamic(physx::PxTransform{ physx::PxIdentity });
	}
	else
	{
		m_pActor = pEngine->GetPhysics()->createRigidStatic(physx::PxTransform{ physx::PxIdentity });
	}

	for (IPhysXShape* pShape : m_pShapes)
	{
		m_pActor->attachShape(pShape->GetShape());
	}

	pPhysXScene->AddActor(m_pActor);
}

void leap::physics::PhysXObject::UpdateTransform()
{
	if (m_pActor == nullptr) return;

	m_IsTransformDirty = false;
	
	const physx::PxVec3 position{ m_Position.x, m_Position.y, m_Position.z };
	const physx::PxQuat rotation{ m_Rotation.x, m_Rotation.y, m_Rotation.z, m_Rotation.w };
	const physx::PxTransform transform{ position, rotation };

	m_pActor->setGlobalPose(transform);
}

void leap::physics::PhysXObject::UpdateRigidbody()
{
	if (m_pRigidbody->isKinematicDirty)
	{
		m_pRigidbody->isKinematicDirty = false;
		m_pActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, m_pRigidbody->isKinematic);
	}

	if (m_pRigidbody->isVelocityDirty)
	{
		m_pRigidbody->isVelocityDirty = false;
		const physx::PxVec3 velocity{ m_pRigidbody->velocity.x, m_pRigidbody->velocity.y, m_pRigidbody->velocity.z };
		static_cast<physx::PxRigidDynamic*>(m_pActor)->setLinearVelocity(velocity);
	}
}
