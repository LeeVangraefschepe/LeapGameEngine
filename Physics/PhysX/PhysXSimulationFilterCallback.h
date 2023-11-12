#pragma once

#include "PhysXSimulationData.h"

#include <PxFiltering.h>

#include <map>

#include <Subject.h>

namespace leap::physics 
{
	class PhysXSimulationFilterCallback final : public physx::PxSimulationFilterCallback
	{
	public:
		PhysXSimulationFilterCallback() = default;
		virtual ~PhysXSimulationFilterCallback() = default;

		PhysXSimulationFilterCallback(const PhysXSimulationFilterCallback& other) = delete;
		PhysXSimulationFilterCallback(PhysXSimulationFilterCallback&& other) = delete;
		PhysXSimulationFilterCallback& operator=(const PhysXSimulationFilterCallback& other) = delete;
		PhysXSimulationFilterCallback& operator=(PhysXSimulationFilterCallback&& other) = delete;

		virtual	physx::PxFilterFlags pairFound(physx::PxU32 pairID,
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, const physx::PxActor* a0, const physx::PxShape* s0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* a1, const physx::PxShape* s1,
			physx::PxPairFlags& pairFlags) override;

		virtual	void pairLost(physx::PxU32 pairID,
			physx::PxFilterObjectAttributes attributes0,
			physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1,
			bool objectRemoved) override;

		virtual	bool statusChange(physx::PxU32& pairID, physx::PxPairFlags& pairFlags, physx::PxFilterFlags& filterFlags) override;

		void NotifyStayingPairs();

		TSubject<SimulationEvent> OnSimulationEvent{};

	private:
		std::map<int, SimulationPair> m_Collisions{};
	};
}