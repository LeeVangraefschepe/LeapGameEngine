#pragma once

#include "IShape.h"
#include "IPhysicsMaterial.h"
#include "../Data/CollisionData.h"

#include <Subject.h>

#include <memory>
#include <functional>

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap::physics
{
	class IPhysicsObject;

	class IPhysics
	{
	public:
		virtual ~IPhysics() = default;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& setFunc, const std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)> getFunc) = 0;
		virtual void Update(float fixedDeltaTime) = 0;

		virtual void CreateScene() = 0;
		virtual IPhysicsObject* Get(void* pOwner) = 0;
		virtual std::unique_ptr<IShape> CreateShape(void* pOwner, EShape shape, IPhysicsMaterial* pMaterial = nullptr) = 0;
		virtual std::shared_ptr<IPhysicsMaterial> CreateMaterial() = 0;

		virtual Subject<CollisionData>& OnCollisionEnter() = 0;
		virtual Subject<CollisionData>& OnCollisionStay() = 0;
		virtual Subject<CollisionData>& OnCollisionExit() = 0;
		virtual Subject<CollisionData>& OnTriggerEnter() = 0;
		virtual Subject<CollisionData>& OnTriggerStay() = 0;
		virtual Subject<CollisionData>& OnTriggerExit() = 0;
	};

	class DefaultPhysics final : public IPhysics
	{
	public:
		virtual ~DefaultPhysics() = default;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>&, const std::function<std::pair<const glm::vec3&, const glm::quat&>(void*)>) override {};
		virtual void Update(float) override {}

		virtual void CreateScene() override {}
		virtual IPhysicsObject* Get(void*) override { return nullptr; }
		virtual std::unique_ptr<IShape> CreateShape(void*, EShape, IPhysicsMaterial*) override { return nullptr; }
		virtual std::shared_ptr<IPhysicsMaterial> CreateMaterial() override { return nullptr; }

		virtual Subject<CollisionData>& OnCollisionEnter() override { return m_EmptyCollision; }
		virtual Subject<CollisionData>& OnCollisionStay() override { return m_EmptyCollision; }
		virtual Subject<CollisionData>& OnCollisionExit() override { return m_EmptyCollision; }
		virtual Subject<CollisionData>& OnTriggerEnter() override { return m_EmptyCollision; }
		virtual Subject<CollisionData>& OnTriggerStay() override { return m_EmptyCollision; }
		virtual Subject<CollisionData>& OnTriggerExit() override { return m_EmptyCollision; }

	private:
		Subject<CollisionData> m_EmptyCollision{};
	};
}