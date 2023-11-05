#include "PhysXSimulationFilterCallback.h"

#include <Debug.h>

physx::PxFilterFlags leap::physics::PhysXSimulationFilterCallback::pairFound(physx::PxU32 pairID,
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData /*filterData0*/, const physx::PxActor* /*a0*/, const physx::PxShape* s0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData /*filterData1*/, const physx::PxActor* /*a1*/, const physx::PxShape* s1,
	physx::PxPairFlags& /*pairFlags*/)
{
	const SimulationPair pair{ s0, s1 };

	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		OnSimulationEvent.Notify({ SimulationEventType::OnTriggerEnter, s0, s1 });
		m_Triggers[pairID] = pair;
	}
	else
	{
		OnSimulationEvent.Notify({ SimulationEventType::OnCollisionEnter, s0, s1 });
		m_Collisions[pairID] = pair;
	}

	return physx::PxFilterFlag::eDEFAULT | physx::PxFilterFlag::eNOTIFY;
}

void leap::physics::PhysXSimulationFilterCallback::pairLost(physx::PxU32 pairID,
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData /*filterData0*/,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData /*filterData1*/,
	bool /*objectRemoved*/)
{
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		const SimulationPair& pair{ m_Triggers[pairID] };

		OnSimulationEvent.Notify({ SimulationEventType::OnTriggerExit, pair.pShape0, pair.pShape1 });
		m_Triggers.erase(pairID);

		return;
	}

	const SimulationPair& pair{ m_Collisions[pairID] };

	OnSimulationEvent.Notify({ SimulationEventType::OnCollissionExit, pair.pShape0, pair.pShape1 });
	m_Collisions.erase(pairID);
}

bool leap::physics::PhysXSimulationFilterCallback::statusChange(physx::PxU32& /*pairID*/, physx::PxPairFlags& /*pairFlags*/, physx::PxFilterFlags& /*filterFlags*/)
{
	return false;
}

void leap::physics::PhysXSimulationFilterCallback::NotifyStayingPairs()
{
	for (const auto& triggerPair : m_Triggers)
	{
		const auto& pair{ triggerPair.second };
		OnSimulationEvent.Notify({ SimulationEventType::OnTriggerStay, pair.pShape0, pair.pShape1 });
	}
	for (const auto& collisionPair : m_Collisions)
	{
		const auto& pair{ collisionPair.second };
		OnSimulationEvent.Notify({ SimulationEventType::OnCollisionStay, pair.pShape0, pair.pShape1 });
	}
}