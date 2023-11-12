#pragma once

#include "PhysXSimulationData.h"

#include <PxSimulationEventCallback.h>

#include <vector>

#include <Subject.h>

namespace physx
{
    class PxRigidActor;
    class PxShape;
}

namespace leap::physics
{
	class PhysXSimulationCallbacks final : public physx::PxSimulationEventCallback
	{
    public:
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

        void NotifyStayingPairs();

        TSubject<SimulationEvent> OnSimulationEvent{};

    private:
        std::vector<SimulationPair> m_Triggers{};
	};
}