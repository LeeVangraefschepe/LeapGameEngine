#include "Rigidbody.h"

void leap::physics::Rigidbody::SetIsKinematic(bool isKinematic)
{
	m_IsKinematic = isKinematic;
	SetDirty(RigidbodyFlag::Kinematic);
}

void leap::physics::Rigidbody::SetVelocity(const glm::vec3& velocity)
{
	m_Velocity = velocity;
	SetDirty(RigidbodyFlag::Velocity);
}

void leap::physics::Rigidbody::SetMass(float mass)
{
	m_Mass = mass;
	SetDirty(RigidbodyFlag::Mass);
}

void leap::physics::Rigidbody::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	SetDirty(RigidbodyFlag::Position);
}

void leap::physics::Rigidbody::Translate(const glm::vec3& translation)
{
	m_Translation += translation;
	SetDirty(RigidbodyFlag::Translate);
}

void leap::physics::Rigidbody::SetRotation(const glm::quat& rotation)
{
	m_Rotation = rotation;
	SetDirty(RigidbodyFlag::Rotation);
}

void leap::physics::Rigidbody::Rotate(const glm::quat& rotationDelta)
{
	m_RotationDelta = rotationDelta * m_RotationDelta;
	SetDirty(RigidbodyFlag::Rotate);
}

void leap::physics::Rigidbody::SetConstraint(Constraint::Flag flag, bool enabled)
{
	auto it{ std::find_if(begin(m_Constraints), end(m_Constraints), [flag](const auto& constraint) { return constraint.flag == flag; }) };

	if (it == end(m_Constraints))
	{
		m_Constraints.emplace_back(Constraint{ flag, enabled });
		SetDirty(RigidbodyFlag::Constraints);
		return;
	}

	if (it->enabled != enabled)
	{
		it->enabled = enabled;
		SetDirty(RigidbodyFlag::Constraints);
	}
}

void leap::physics::Rigidbody::AddForce(const glm::vec3& force, leap::physics::ForceMode mode)
{
	m_Forces.emplace_back(Force{ force, false, mode });
}

void leap::physics::Rigidbody::AddTorque(const glm::vec3& torque, ForceMode mode)
{
	m_Forces.emplace_back(Force{ torque, true, mode });
}

bool leap::physics::Rigidbody::IsDirty() const
{
	return static_cast<unsigned int>(m_DirtyFlag) > 0 || !m_Forces.empty();
}

leap::physics::Rigidbody::RigidbodyFlag leap::physics::Rigidbody::GetDirtyFlag() const
{
	return m_DirtyFlag;
}

void leap::physics::Rigidbody::ResetDirtyFlag()
{
	m_DirtyFlag = RigidbodyFlag::None;
	m_RotationDelta = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_Translation = {};
}

void leap::physics::Rigidbody::SetDirty(RigidbodyFlag flag)
{
	m_DirtyFlag = static_cast<RigidbodyFlag>(static_cast<unsigned int>(m_DirtyFlag) | static_cast<unsigned int>(flag));
}