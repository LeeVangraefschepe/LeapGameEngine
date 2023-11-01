#pragma once

#include "../Component.h"

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap
{
	namespace physics
	{
		class Rigidbody;
	}

	class Rigidbody final : public Component
	{
	public:
		Rigidbody() = default;
		virtual ~Rigidbody() = default;

		Rigidbody(const Rigidbody& other) = delete;
		Rigidbody(Rigidbody&& other) = delete;
		Rigidbody& operator=(const Rigidbody& other) = delete;
		Rigidbody& operator=(Rigidbody&& other) = delete;

		void SetKinematic(bool isKinematic);
		void SetVelocity(const glm::vec3& velocity);
		void SetVelocity(float x, float y, float z);
		void SetMass(float mass);

		void Translate(const glm::vec3& displacement) const;
		void Translate(float x, float y, float z) const;
		void SetPosition(const glm::vec3& position) const;
		void SetPosition(float x, float y, float z) const;
		void Rotate(const glm::quat& rotationEpsilon) const;
		void Rotate(const glm::vec3& rotationEpsilon, bool degrees = true) const;
		void Rotate(float x, float y, float z, bool degrees = true) const;
		void SetRotation(const glm::quat& rotation) const;
		void SetRotation(const glm::vec3& rotation, bool degrees = true) const;
		void SetRotation(float x, float y, float z, bool degrees = true) const;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void ApplyShapes(GameObject* pParent) const;

		physics::Rigidbody* m_pRigidbody{};
	};
}