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
	class PxDefaultCpuDispatcher;
}

namespace leap::physics
{
	class IPhysicsScene;

	class PhysXEngine final : public IPhysics
	{
	public:
		PhysXEngine();
		virtual ~PhysXEngine();

		PhysXEngine(const PhysXEngine& other) = delete;
		PhysXEngine(PhysXEngine&& other) = delete;
		PhysXEngine& operator=(const PhysXEngine& other) = delete;
		PhysXEngine& operator=(PhysXEngine&& other) = delete;

		virtual void CreateScene() override;

	private:
		std::unique_ptr<physx::PxDefaultErrorCallback> m_pDefaultErrorCallback{};
		std::unique_ptr<physx::PxDefaultAllocator> m_pDefaultAllocatorCallback{};

		physx::PxFoundation* m_pFoundation{};
		physx::PxPhysics* m_pPhysics{};
		physx::PxCooking* m_pCooking{};
		physx::PxDefaultCpuDispatcher* m_pDispatcher{};

		std::unique_ptr<physics::IPhysicsScene> m_pScene{};
	};
}