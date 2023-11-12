#include "PhysXSimulationCallbacks.h"

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

void leap::physics::PhysXSimulationCallbacks::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (unsigned int i{}; i < count; ++i)
	{
		const physx::PxTriggerPair& pair{ pairs[i] };

		switch (pair.status)
		{
		case physx::PxPairFlag::Enum::eNOTIFY_TOUCH_FOUND:
		{	
			OnSimulationEvent.Notify({ SimulationEventType::OnTriggerEnter, pair.triggerShape, pair.otherShape });
			m_Triggers.emplace_back(SimulationPair{ pair.triggerShape, pair.otherShape });
			break;
		}
		case physx::PxPairFlag::Enum::eNOTIFY_TOUCH_LOST:
		{
			const auto pairIt{ std::find_if(begin(m_Triggers), end(m_Triggers), [&](const auto& triggerPair) { return (triggerPair.pShape0 == pair.triggerShape || triggerPair.pShape0 == pair.otherShape) && (triggerPair.pShape1 == pair.triggerShape || triggerPair.pShape1 == pair.otherShape); }) };

			OnSimulationEvent.Notify({ SimulationEventType::OnTriggerExit, pairIt->pShape0, pairIt->pShape1 });
			m_Triggers.erase(pairIt);
			break;
		}
		}
	}
}

void leap::physics::PhysXSimulationCallbacks::onAdvance(const physx::PxRigidBody* const* /*bodyBuffer*/, const physx::PxTransform* /*poseBuffer*/, const physx::PxU32 /*count*/)
{
}

void leap::physics::PhysXSimulationCallbacks::NotifyStayingPairs()
{
	for (const auto& triggerPair : m_Triggers)
	{
		OnSimulationEvent.Notify({ SimulationEventType::OnTriggerStay, triggerPair.pShape0, triggerPair.pShape1 });
	}
}
