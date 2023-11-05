#pragma once

#include <PxFiltering.h>

#include <map>

#include <Subject.h>

namespace leap::physics 
{
	class PhysXSimulationFilterCallback final : public physx::PxSimulationFilterCallback
	{
	public:
		enum class SimulationEventType
		{
			OnTriggerEnter,
			OnTriggerStay,
			OnTriggerExit,
			OnCollisionEnter,
			OnCollisionStay,
			OnCollissionExit
		};

		struct SimulationEvent final
		{
			SimulationEventType type{};
			const physx::PxShape* pShape0{};
			const physx::PxShape* pShape1{};
		};

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

		Subject<SimulationEvent> OnSimulationEvent{};

	private:
		struct SimulationPair final
		{
			const physx::PxShape* pShape0{};
			const physx::PxShape* pShape1{};
		};

		std::map<int, SimulationPair> m_Collisions{};
		std::map<int, SimulationPair> m_Triggers{};
	};
}