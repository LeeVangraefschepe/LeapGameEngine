#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)
#include <Quaternion.h>

#include "../Data/Force.h"

#include <vector>
#include <functional>

namespace leap::physics
{
	class PhysXObject;

	class Rigidbody
	{
	public:
		Rigidbody() = default;
		Rigidbody(const std::function<void()>& rigidbodyRequestFunc);

		Rigidbody& operator=(const Rigidbody& other) = delete;
		Rigidbody& operator=(Rigidbody&& other) noexcept;
		Rigidbody(const Rigidbody& other) = delete;
		Rigidbody(Rigidbody&& other) = delete;

		enum class RigidbodyFlag
		{
			None = 0,
			Kinematic = 1,
			Velocity = 2,
			Mass = 4,
			Position = 8,
			Rotation = 16,
			Translate = 32,
			Rotate = 64,
			Constraints = 128,
			AngularVelocity = 256
		};
		struct Constraint
		{
			enum class Flag
			{
				MovementX = 1,
				MovementY = 2,
				MovementZ = 4,
				RotationX = 8,
				RotationY = 16,
				RotationZ = 32
			};
			Flag flag{};
			bool enabled{};
		};

		void SetIsKinematic(bool isKinematic);
		void SetVelocity(const glm::vec3& velocity);
		void SetAngularVelocity(const glm::vec3& velocity);
		void SetMass(float mass);
		void SetPosition(const glm::vec3& position);
		void Translate(const glm::vec3& translation);
		void SetRotation(const glm::quat& rotation);
		void Rotate(const glm::quat& rotationDelta);
		void SetConstraint(Constraint::Flag flag, bool enabled);

		bool IsKinematic() const { return m_IsKinematic; }
		const glm::vec3& GetVelocity() const { return m_Velocity; }
		const glm::vec3& GetAngularVelocity() const { return m_AngularVelocity; }
		float GetMass() const { return m_Mass; }
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::quat& GetRotation() const { return m_Rotation; }
		const glm::quat& GetRotationDelta() const { return m_RotationDelta; }
		const std::vector<Constraint>& GetConstraints() { return m_Constraints; }

		const glm::vec3& GetVelocityFromEngine();
		const glm::vec3& GetAngularVelocityFromEngine();

		void AddForce(const glm::vec3& force, ForceMode mode);
		void AddTorque(const glm::vec3& torque, ForceMode mode);
		std::vector<Force>& GetForces() { return m_Forces; }

		bool IsDirty() const;
		RigidbodyFlag GetDirtyFlag() const;
		void ResetDirtyFlag();

	private:
		friend PhysXObject;

		void SetVelocityFromEngine(const glm::vec3& velocity);
		void SetAngularVelocityFromEngine(const glm::vec3& velocity);

		void SetDirty(RigidbodyFlag flag);

		bool m_IsKinematic{};
		glm::vec3 m_Velocity{};
		glm::vec3 m_AngularVelocity{};
		float m_Mass{ 1.0f };
		glm::vec3 m_Position{};
		glm::quat m_Rotation{ Quaternion::Identity() };
		glm::vec3 m_Translation{};
		glm::quat m_RotationDelta{ Quaternion::Identity() };
		RigidbodyFlag m_DirtyFlag{ RigidbodyFlag::None };
		std::vector<Force> m_Forces{};
		std::vector<Constraint> m_Constraints{};

		std::function<void()> m_UpdateRequestFunc{};
	};
}