#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

#include "../Data/Force.h"

#include <vector>

namespace leap::physics
{
	class Rigidbody
	{
	public:
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
			Constraints = 128
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
		void SetMass(float mass);
		void SetPosition(const glm::vec3& position);
		void Translate(const glm::vec3& translation);
		void SetRotation(const glm::quat& rotation);
		void Rotate(const glm::quat& rotationDelta);
		void SetConstraint(Constraint::Flag flag, bool enabled);

		bool IsKinematic() const { return m_IsKinematic; }
		const glm::vec3& GetVelocity() const { return m_Velocity; }
		float GetMass() const { return m_Mass; }
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::quat& GetRotation() const { return m_Rotation; }
		const glm::quat& GetRotationDelta() const { return m_RotationDelta; }
		const std::vector<Constraint> GetConstraints() { return m_Constraints; }

		void AddForce(const glm::vec3& force, ForceMode mode);
		void AddTorque(const glm::vec3& torque, ForceMode mode);
		std::vector<Force>& GetForces() { return m_Forces; }

		bool IsDirty() const;
		RigidbodyFlag GetDirtyFlag() const;
		void ResetDirtyFlag();

	private:
		void SetDirty(RigidbodyFlag flag);

		bool m_IsKinematic{};
		glm::vec3 m_Velocity{};
		float m_Mass{ 1.0f };
		glm::vec3 m_Position{};
		glm::quat m_Rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Translation{};
		glm::quat m_RotationDelta{ 1.0f, 0.0f, 0.0f, 0.0f };
		RigidbodyFlag m_DirtyFlag{ RigidbodyFlag::None };
		std::vector<Force> m_Forces{};
		std::vector<Constraint> m_Constraints{};
	};
}