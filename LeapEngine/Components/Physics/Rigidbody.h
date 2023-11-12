#pragma once

#include "../Component.h"

#include <Data/Rigidbody.h>

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

#include <Data/ForceMode.h>

namespace leap
{
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
		void SetAngularVelocity(const glm::vec3& velocity);
		void SetAngularVelocity(float x, float y, float z);
		void SetMass(float mass);
		void SetConstraint(physics::Rigidbody::Constraint::Flag flag, bool enabled);

		void Translate(const glm::vec3& displacement);
		void Translate(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);
		void Rotate(const glm::quat& rotationEpsilon);
		void Rotate(const glm::vec3& rotationEpsilon, bool degrees = true);
		void Rotate(float x, float y, float z, bool degrees = true);
		void SetRotation(const glm::quat& rotation);
		void SetRotation(const glm::vec3& rotation, bool degrees = true);
		void SetRotation(float x, float y, float z, bool degrees = true);

		void AddForce(const glm::vec3& force, physics::ForceMode mode = physics::ForceMode::Force);
		void AddForce(float x, float y, float z, physics::ForceMode mode = physics::ForceMode::Force);

		void AddTorque(const glm::vec3& torque, physics::ForceMode mode = physics::ForceMode::Force);
		void AddTorque(float x, float y, float z, physics::ForceMode mode = physics::ForceMode::Force);

		bool IsKinematic() const { return m_pRigidbody->IsKinematic(); }
		const glm::vec3& GetVelocity() const;
		const glm::vec3& GetAngularVelocity() const;
		float GetMass() const { return m_pRigidbody->GetMass(); }
		bool IsConstraint(physics::Rigidbody::Constraint::Flag flag, bool enabled) const;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void CheckExistence();
		void ApplyShapes(GameObject* pParent) const;

		physics::Rigidbody* m_pRigidbody{};

		inline static glm::vec3 m_EmptyVector{};
	};
}