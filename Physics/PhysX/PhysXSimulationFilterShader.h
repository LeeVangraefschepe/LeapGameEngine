#pragma once

#include <PxFiltering.h>

namespace leap::physics
{
	inline physx::PxFilterFlags PhysXSimulationFilterShader(
		physx::PxFilterObjectAttributes /*attribute0*/, physx::PxFilterData /*filterData0*/,
		physx::PxFilterObjectAttributes /*attribute1*/, physx::PxFilterData /*filterData1*/,
		physx::PxPairFlags& pairFlags, const void* /*pConstantBlock*/, physx::PxU32 /*blockSize*/)
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;

		return physx::PxFilterFlag::eDEFAULT;
	}
}