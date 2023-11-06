#pragma once

#include <PxFiltering.h>

namespace leap::physics
{
	inline physx::PxFilterFlags PhysXSimulationFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData /*filterData0*/,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData /*filterData1*/,
		physx::PxPairFlags& pairFlags, const void* /*pConstantBlock*/, physx::PxU32 /*blockSize*/)
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eMODIFY_CONTACTS;
		pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;

		return physx::PxFilterFlag::eCALLBACK;
	}
}