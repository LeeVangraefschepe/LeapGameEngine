#pragma once

#include "../Interfaces/IPhysics.h"
#include "../Interfaces/IPhysicsObject.h"
#include "../Interfaces/IShape.h"

#include <memory>
#include <unordered_map>

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

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
	class PhysXObject;

	class PhysXEngine final : public IPhysics
	{
	public:
		PhysXEngine();
		virtual ~PhysXEngine();

		PhysXEngine(const PhysXEngine& other) = delete;
		PhysXEngine(PhysXEngine&& other) = delete;
		PhysXEngine& operator=(const PhysXEngine& other) = delete;
		PhysXEngine& operator=(PhysXEngine&& other) = delete;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& setFunc, const std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)> getFunc) override;
		virtual void Update(float fixedDeltaTime) override;

		virtual void CreateScene() override;
		virtual IPhysicsObject* Get(void* pOwner) override;
		virtual std::unique_ptr<IShape> CreateShape(EShape shape, IPhysicsMaterial* pMaterial = nullptr) override;
		virtual std::shared_ptr<IPhysicsMaterial> CreateMaterial() override;

		physx::PxPhysics* GetPhysics() const { return m_pPhysics; }

	private:
		IPhysicsMaterial* GetDefaultMaterial();

		std::unique_ptr<physx::PxDefaultErrorCallback> m_pDefaultErrorCallback{};
		std::unique_ptr<physx::PxDefaultAllocator> m_pDefaultAllocatorCallback{};

		physx::PxFoundation* m_pFoundation{};
		physx::PxPhysics* m_pPhysics{};
		physx::PxCooking* m_pCooking{};
		physx::PxDefaultCpuDispatcher* m_pDispatcher{};

		std::unique_ptr<physics::IPhysicsScene> m_pScene{};

		std::unordered_map<void*, std::unique_ptr<PhysXObject>> m_pObjects{};

		std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)> m_SyncGetFunc{};
		std::function<void(void*, const glm::vec3&, const glm::quat&)> m_SyncSetFunc{};
	};
}