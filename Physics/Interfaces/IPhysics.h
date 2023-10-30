#pragma once

#include "IShape.h"

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

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>& syncFunc) = 0;
		virtual void Update(float fixedDeltaTime) = 0;

		virtual void CreateScene() = 0;
		virtual IPhysicsObject* Get(void* pOwner) = 0;
		virtual std::unique_ptr<IShape> CreateShape(EShape shape) = 0;
	};

	class DefaultPhysics final : public IPhysics
	{
	public:
		virtual ~DefaultPhysics() = default;

		virtual void SetSyncFunc(const std::function<void(void*, const glm::vec3&, const glm::quat&)>&) override {};
		virtual void Update(float) override {}

		virtual void CreateScene() override {}
		virtual IPhysicsObject* Get(void*) override { return nullptr; }
		virtual std::unique_ptr<IShape> CreateShape(EShape) override { return nullptr; }
	};
}