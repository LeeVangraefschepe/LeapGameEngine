#include "PhysXEngine.h"

#include "PxPhysicsAPI.h"

#include <Debug.h>

leap::physics::PhysXEngine::PhysXEngine()
    : m_pDefaultAllocatorCallback{ std::make_unique<physx::PxDefaultAllocator>() }
    , m_pDefaultErrorCallback{ std::make_unique<physx::PxDefaultErrorCallback>() }
{
    // Create foundation
    m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pDefaultAllocatorCallback, *m_pDefaultErrorCallback);
    if (!m_pFoundation)
    {
        Debug::LogError("PhysXEngine Error : PxCreateFoundation failed");
        return;
    }

    m_pDefaultAllocatorCallback->allocate(3, nullptr, nullptr, 5);

    // Create top-level physics object
    bool recordMemoryAllocations{};
#if _DEBUG
    recordMemoryAllocations = true;
#endif
    physx::PxTolerancesScale scaling{}; // Default scaling (size = 1 & velocity = gravity * 1s = 9.81)
    scaling.length = 1.0f;
    scaling.speed = 9.81f;
    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, scaling, recordMemoryAllocations, nullptr);
    if (!m_pPhysics)
    {
        Debug::LogError("PhysXEngine Error : PxCreatePhysics failed");
        return;
    }

    // Create cooking object
    m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(m_pPhysics->getTolerancesScale()));
    if (!m_pCooking)
    {
        Debug::LogError("PhysXEngine Error : PxCreateCooking failed");
        return;
    }
}

leap::physics::PhysXEngine::~PhysXEngine()
{
    m_pCooking->release();
    m_pPhysics->release();
    m_pFoundation->release();
}
