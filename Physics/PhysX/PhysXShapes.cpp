#include "PhysXShapes.h"

#include "PhysXEngine.h"

#include <PxPhysics.h>

#include <Debug.h>

leap::physics::PhysXBoxShape::PhysXBoxShape(PhysXEngine* pEngine)
{
	physx::PxBoxGeometry geo{ 0.5f, 0.5f, 0.5f };

	physx::PxMaterial* material{ pEngine->GetPhysics()->createMaterial(0.1f, 0.1f, 0.0f) };

	m_pShape = pEngine->GetPhysics()->createShape(geo, *material, true);
}

leap::physics::PhysXBoxShape::~PhysXBoxShape()
{
	m_pShape->release();
}

physx::PxShape& leap::physics::PhysXBoxShape::GetShape()
{
	return *m_pShape;
}

void leap::physics::PhysXBoxShape::SetSize(const glm::vec3& size)
{
	m_pShape->setGeometry(physx::PxBoxGeometry{ size.x / 2.0f, size.y / 2.0f, size.z / 2.0f });
}

void leap::physics::PhysXBoxShape::SetRadius(float)
{
	Debug::LogWarning("PhysXEngine Warning: Cannot set radius on a box shape");
}

void leap::physics::PhysXBoxShape::SetRelativeTransform(const glm::vec3& position, const glm::quat& rotation)
{
	m_pShape->setLocalPose(physx::PxTransform{ physx::PxVec3{ position.x, position.y, position.z }, physx::PxQuat{ rotation.x, rotation.y, rotation.z,rotation.w } });
}

leap::physics::PhysXSphereShape::PhysXSphereShape(PhysXEngine* pEngine)
{
	physx::PxSphereGeometry geo{ 0.5f };

	physx::PxMaterial* material{ pEngine->GetPhysics()->createMaterial(0.1f, 0.1f, 0.0f) };

	m_pShape = pEngine->GetPhysics()->createShape(geo, *material, true);
}

leap::physics::PhysXSphereShape::~PhysXSphereShape()
{
	m_pShape->release();
}

physx::PxShape& leap::physics::PhysXSphereShape::GetShape()
{
	return *m_pShape;
}

void leap::physics::PhysXSphereShape::SetRadius(float radius)
{
	m_pShape->setGeometry(physx::PxSphereGeometry{ radius });
}

void leap::physics::PhysXSphereShape::SetRelativeTransform(const glm::vec3& position, const glm::quat& rotation)
{
	m_pShape->setLocalPose(physx::PxTransform{ physx::PxVec3{ position.x, position.y, position.z }, physx::PxQuat{ rotation.x, rotation.y, rotation.z,rotation.w } });
}

void leap::physics::PhysXSphereShape::SetSize(const glm::vec3&)
{
	Debug::LogWarning("PhysXEngine Warning: Cannot set size on a sphere shape");
}
