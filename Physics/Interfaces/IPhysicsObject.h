#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap::physics
{
	class IShape;

	class IPhysicsObject
	{
	public:
		struct Rigidbody
		{
			bool enabled{};
			bool isKinematic{};
			glm::vec3 velocity{};
			bool isKinematicDirty{};
			bool isVelocityDirty{};

			bool IsDirty() const { return isKinematicDirty || isVelocityDirty; }
		};

		virtual ~IPhysicsObject() = default;

		virtual void AddShape(IShape* pShape) = 0;
		virtual void RemoveShape(IShape* pShape) = 0;
		virtual Rigidbody* SetRigidbody(bool hasRigidbody) = 0;

		virtual void SetTransform(const glm::vec3& position, const glm::quat& rotation) = 0;
	};
}