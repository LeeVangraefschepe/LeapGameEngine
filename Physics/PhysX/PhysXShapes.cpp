#include "PhysXShapes.h"

#include "PhysXEngine.h"

#include <PxPhysics.h>

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
