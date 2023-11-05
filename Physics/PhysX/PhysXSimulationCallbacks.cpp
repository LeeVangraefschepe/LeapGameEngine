#include "PhysXSimulationCallbacks.h"

#include <Debug.h>
#include <thread>
#include <iostream>

void leap::physics::PhysXSimulationCallbacks::onConstraintBreak(physx::PxConstraintInfo* /*constraints*/, physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onWake(physx::PxActor** /*actors*/, physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onSleep(physx::PxActor** /*actors*/, physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onContact(const physx::PxContactPairHeader& /*pairHeader*/, const physx::PxContactPair* /*pairs*/, physx::PxU32 /*nbPairs*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onTrigger(physx::PxTriggerPair* /*pairs*/, physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onAdvance(const physx::PxRigidBody* const* /*bodyBuffer*/, const physx::PxTransform* /*poseBuffer*/, const physx::PxU32 /*count*/)
{
}
