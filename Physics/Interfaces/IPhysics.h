#pragma once

#include "IShape.h"
#include "IPhysicsMaterial.h"
#include "../Data/CollisionData.h"

#include <Delegate.h>

#include <memory>
#include <functional>

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap::physics
{
	class IPhysicsObject;
	struct RaycastHit;

	DECLARE_DELEGATE_OneParam(OnCollisionEnter, const CollisionData&);
	DECLARE_DELEGATE_OneParam(OnCollisionStay, const CollisionData&);
	DECLARE_DELEGATE_OneParam(OnCollisionExit, const CollisionData&);
	DECLARE_DELEGATE_OneParam(OnTriggerEnter, const CollisionData&);
	DECLARE_DELEGATE_OneParam(OnTriggerStay, const CollisionData&);
	DECLARE_DELEGATE_OneParam(OnTriggerExit, const CollisionData&);

	class IPhysics
	{
	public:
		virtual ~IPhysics() = default;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& setFunc, const std::function<std::pair<glm::vec3, glm::quat>(void*)> getFunc) = 0;
		virtual void Update(float fixedDeltaTime) = 0;

		virtual void CreateScene() = 0;
		virtual IPhysicsObject* Get(void* pOwner) = 0;
		virtual std::unique_ptr<IShape> CreateShape(void* pOwner, EShape shape, IPhysicsMaterial* pMaterial = nullptr) = 0;
		virtual std::shared_ptr<IPhysicsMaterial> CreateMaterial() = 0;

		virtual void SetEnabledDebugDrawing(bool isEnabled) = 0;
		virtual std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugDrawings() = 0;

		OnCollisionEnter OnCollisionEnterDelegate;
		OnCollisionStay OnCollisionStayDelegate;
		OnCollisionExit OnCollisionExitDelegate;
		OnTriggerEnter OnTriggerEnterDelegate;
		OnTriggerStay OnTriggerStayDelegate;
		OnTriggerExit OnTriggerExitDelegate;

		virtual bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHit& hitInfo) = 0;
	};

	class DefaultPhysics final : public IPhysics
	{
	public:
		virtual ~DefaultPhysics() = default;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>&, const std::function<std::pair<glm::vec3, glm::quat>(void*)>) override {};
		virtual void Update(float) override {}

		virtual void CreateScene() override {}
		virtual IPhysicsObject* Get(void*) override { return nullptr; }
		virtual std::unique_ptr<IShape> CreateShape(void*, EShape, IPhysicsMaterial*) override { return nullptr; }
		virtual std::shared_ptr<IPhysicsMaterial> CreateMaterial() override { return nullptr; }

		virtual void SetEnabledDebugDrawing(bool) override {}
		virtual std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugDrawings() { return {}; }

		virtual bool Raycast(const glm::vec3&, const glm::vec3&, float, RaycastHit&) override { return {}; }
	};
}