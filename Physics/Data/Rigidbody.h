#pragma once

#include <vec3.hpp>

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
			Mass = 4
		};

		void SetIsKinematic(bool isKinematic);
		void SetVelocity(const glm::vec3& velocity);
		void SetMass(float mass);

		bool IsKinematic() const;
		const glm::vec3& GetVelocity() const;
		float GetMass() const;

		bool IsDirty() const;
		RigidbodyFlag GetDirtyFlag() const;
		void ResetDirtyFlag();

	private:
		void SetDirty(RigidbodyFlag flag);

		bool m_IsKinematic{};
		glm::vec3 m_Velocity{};
		float m_Mass{ 1.0f };
		RigidbodyFlag m_DirtyFlag{ RigidbodyFlag::None };
	};
}