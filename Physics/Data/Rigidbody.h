#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

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
			Rotate = 64
		};

		void SetIsKinematic(bool isKinematic);
		void SetVelocity(const glm::vec3& velocity);
		void SetMass(float mass);
		void SetPosition(const glm::vec3& position);
		void Translate(const glm::vec3& translation);
		void SetRotation(const glm::quat& rotation);
		void Rotate(const glm::quat& rotationDelta);

		bool IsKinematic() const { return m_IsKinematic; }
		const glm::vec3& GetVelocity() const { return m_Velocity; }
		float GetMass() const { return m_Mass; }
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::quat& GetRotation() const { return m_Rotation; }
		const glm::quat& GetRotationDelta() const { return m_RotationDelta; }

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
	};
}