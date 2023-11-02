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

void leap::physics::PhysXSimulationCallbacks::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	CollisionEvent e{};

	e.pFirst = pairHeader.actors[0];
	e.pSecond = pairHeader.actors[1];

	for (int i{}; i < static_cast<int>(nbPairs); ++i)
	{
		const physx::PxContactPair& pair{ pairs[i] };

		e.pFirstShapes.emplace_back(pair.shapes[0]);
		e.pSecondShapes.emplace_back(pair.shapes[1]);
	}

	OnCollision.Notify(e);
}

void leap::physics::PhysXSimulationCallbacks::onTrigger(physx::PxTriggerPair* /*pairs*/, physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::onAdvance(const physx::PxRigidBody* const* /*bodyBuffer*/, const physx::PxTransform* /*poseBuffer*/, const physx::PxU32 /*count*/)
{
}
