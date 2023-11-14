#include "Rigidbody.h"

#include <Quaternion.h>

leap::physics::Rigidbody::Rigidbody(const std::function<void()>& rigidbodyRequestFunc)
	: m_UpdateRequestFunc{ rigidbodyRequestFunc }
{
}

leap::physics::Rigidbody& leap::physics::Rigidbody::operator=(Rigidbody&& other) noexcept
{
	m_IsKinematic = other.m_IsKinematic;
	m_Velocity = other.m_Velocity;
	m_AngularVelocity = other.m_AngularVelocity;
	m_Mass = other.m_Mass;
	m_Position = other.m_Position;
	m_Rotation = other.m_Rotation;
	m_Translation = other.m_Translation;
	m_RotationDelta = other.m_RotationDelta;
	m_DirtyFlag = other.m_DirtyFlag;
	m_Forces = other.m_Forces;
	m_Constraints = other.m_Constraints;

	if (other.m_UpdateRequestFunc) m_UpdateRequestFunc = other.m_UpdateRequestFunc;

	return *this;
}

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

void leap::physics::Rigidbody::SetAngularVelocity(const glm::vec3& velocity)
{
	m_AngularVelocity = velocity;
	SetDirty(RigidbodyFlag::AngularVelocity);
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

const glm::vec3& leap::physics::Rigidbody::GetVelocityFromEngine()
{
	if (!(static_cast<unsigned int>(m_DirtyFlag) & static_cast<unsigned int>(Rigidbody::RigidbodyFlag::Velocity)))
	{
		if (m_UpdateRequestFunc)m_UpdateRequestFunc();
	}

	return m_Velocity;
}

const glm::vec3& leap::physics::Rigidbody::GetAngularVelocityFromEngine()
{
	if (!(static_cast<unsigned int>(m_DirtyFlag) & static_cast<unsigned int>(Rigidbody::RigidbodyFlag::AngularVelocity)))
	{
		if(m_UpdateRequestFunc) m_UpdateRequestFunc();
	}

	return m_AngularVelocity;
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
	m_RotationDelta = Quaternion::Identity();
	m_Translation = {};
}

void leap::physics::Rigidbody::SetVelocityFromEngine(const glm::vec3& velocity)
{
	m_Velocity = velocity;
}

void leap::physics::Rigidbody::SetAngularVelocityFromEngine(const glm::vec3& velocity)
{
	m_AngularVelocity = velocity;
}

void leap::physics::Rigidbody::SetDirty(RigidbodyFlag flag)
{
	m_DirtyFlag = static_cast<RigidbodyFlag>(static_cast<unsigned int>(m_DirtyFlag) | static_cast<unsigned int>(flag));
}
