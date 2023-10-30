#include "PhysXEngine.h"

#include "PxPhysicsAPI.h"

#include <Debug.h>

#include "PhysXScene.h"
#include "PhysXShapes.h"
#include "PhysXObject.h"

#include <algorithm>

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

    // Create a CPU dispatcher
    m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
}

leap::physics::PhysXEngine::~PhysXEngine()
{
    m_pScene = nullptr;
    m_pObjects.clear();

    m_pCooking->release();
    m_pPhysics->release();
    m_pFoundation->release();
}

void leap::physics::PhysXEngine::SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& setFunc, const std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)> getFunc)
{
    m_SyncGetFunc = getFunc;
    m_SyncSetFunc = setFunc;
}

void leap::physics::PhysXEngine::Update(float fixedDeltaTime)
{
    // Update all the physics objects and apply updates
    for (auto& pObject : m_pObjects)
    {
        pObject.second->Update(this, m_pScene.get());
    }

    // Erase all objects that are not connected anymore to a game object
    std::erase_if(m_pObjects, [](const auto& pObject) { return !pObject.second->IsValid(); });

    // Simulate the physics scene
    m_pScene->Simulate(fixedDeltaTime);

    // Apply poses
    for (auto& pObject : m_pObjects)
    {
        pObject.second->Apply(m_SyncSetFunc, m_SyncGetFunc);
    }
}

void leap::physics::PhysXEngine::CreateScene()
{
    physx::PxSceneDesc sceneDesc{ m_pPhysics->getTolerancesScale() };
    sceneDesc.gravity = physx::PxVec3{ 0.0f, -9.81f, 0.0f };
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    sceneDesc.cpuDispatcher = m_pDispatcher;

    physx::PxScene* pPhysXScene{ m_pPhysics->createScene(sceneDesc) };

    m_pScene = std::make_unique<PhysXScene>(pPhysXScene);
}

leap::physics::IPhysicsObject* leap::physics::PhysXEngine::Get(void* pOwner)
{
    if (!m_pObjects.contains(pOwner))
    {
        m_pObjects[pOwner] = std::make_unique<PhysXObject>(pOwner);
    }

    return m_pObjects[pOwner].get();
}

std::unique_ptr<leap::physics::IShape> leap::physics::PhysXEngine::CreateShape(physics::EShape shape)
{
    switch (shape)
    {
    case EShape::Box:
        return std::make_unique<PhysXBoxShape>(this);
    case EShape::Sphere:
        return std::make_unique<PhysXSphereShape>(this);
    }

    return nullptr;
}