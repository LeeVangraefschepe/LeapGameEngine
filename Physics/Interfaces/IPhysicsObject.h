#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap::physics
{
	class IShape;
	class Rigidbody;

	class IPhysicsObject
	{
	public:
		virtual ~IPhysicsObject() = default;

		virtual void AddShape(IShape* pShape) = 0;
		virtual void RemoveShape(IShape* pShape) = 0;
		virtual Rigidbody* SetRigidbody(bool hasRigidbody) = 0;
		virtual bool IsValid() = 0;

		virtual void SetTransform(const glm::vec3& position, const glm::quat& rotation) = 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::quat GetRotation() = 0;
	};
}