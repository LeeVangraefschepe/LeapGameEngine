#pragma once

#include "../Interfaces/IPhysics.h"

#include <memory>

namespace physx
{
	class PxDefaultErrorCallback;
	class PxDefaultAllocator;
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
}

namespace leap::physics
{
	class PhysXEngine final : public IPhysics
	{
	public:
		PhysXEngine();
		virtual ~PhysXEngine();

		PhysXEngine(const PhysXEngine& other) = delete;
		PhysXEngine(PhysXEngine&& other) = delete;
		PhysXEngine& operator=(const PhysXEngine& other) = delete;
		PhysXEngine& operator=(PhysXEngine&& other) = delete;

	private:
		std::unique_ptr<physx::PxDefaultErrorCallback> m_pDefaultErrorCallback{};
		std::unique_ptr<physx::PxDefaultAllocator> m_pDefaultAllocatorCallback{};

		physx::PxFoundation* m_pFoundation{};
		physx::PxPhysics* m_pPhysics{};
		physx::PxCooking* m_pCooking{};
	};
}