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

		virtual Subject<CollisionData>& OnCollision() = 0;
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

		virtual Subject<CollisionData>& OnCollision() { return m_OnCollision; }

	private:
		Subject<CollisionData> m_OnCollision{};
	};
}