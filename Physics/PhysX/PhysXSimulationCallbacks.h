#pragma once

#include <PxSimulationEventCallback.h>

#include <Subject.h>

namespace physx
{
    class PxRigidActor;
    class PxShape;
}

namespace leap::physics
{
    class PhysXEngine;

	class PhysXSimulationCallbacks final : public physx::PxSimulationEventCallback
	{
    public:
        struct CollisionEvent final
        {
            physx::PxRigidActor* pFirst{};
            std::vector<physx::PxShape*> pFirstShapes{};
            physx::PxRigidActor* pSecond{};
            std::vector<physx::PxShape*> pSecondShapes{};
        };

        PhysXSimulationCallbacks() = default;
        virtual ~PhysXSimulationCallbacks() = default;

        PhysXSimulationCallbacks(const PhysXSimulationCallbacks& other) = delete;
        PhysXSimulationCallbacks(PhysXSimulationCallbacks&& other) = delete;
        PhysXSimulationCallbacks& operator=(const PhysXSimulationCallbacks& other) = delete;
        PhysXSimulationCallbacks& operator=(PhysXSimulationCallbacks&& other) = delete;

        virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
        virtual void onWake(physx::PxActor * *actors, physx::PxU32 count) override;
        virtual void onSleep(physx::PxActor * *actors, physx::PxU32 count) override;
        virtual void onContact(const physx::PxContactPairHeader & pairHeader, const physx::PxContactPair * pairs, physx::PxU32 nbPairs) override;
        virtual void onTrigger(physx::PxTriggerPair * pairs, physx::PxU32 count) override;
        virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform * poseBuffer, const physx::PxU32 count) override;

        Subject<CollisionEvent> OnCollision{};
        Subject<CollisionEvent> OnTrigger{};
	};
}