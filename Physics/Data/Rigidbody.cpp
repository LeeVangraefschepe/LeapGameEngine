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

bool leap::physics::Rigidbody::IsKinematic() const
{
	return m_IsKinematic;
}

const glm::vec3& leap::physics::Rigidbody::GetVelocity() const
{
	return m_Velocity;
}

float leap::physics::Rigidbody::GetMass() const
{
	return m_Mass;
}

bool leap::physics::Rigidbody::IsDirty() const
{
	return static_cast<unsigned int>(m_DirtyFlag) > 0;
}

leap::physics::Rigidbody::RigidbodyFlag leap::physics::Rigidbody::GetDirtyFlag() const
{
	return m_DirtyFlag;
}

void leap::physics::Rigidbody::ResetDirtyFlag()
{
	m_DirtyFlag = RigidbodyFlag::None;
}

void leap::physics::Rigidbody::SetDirty(RigidbodyFlag flag)
{
	m_DirtyFlag = static_cast<RigidbodyFlag>(static_cast<unsigned int>(m_DirtyFlag) | static_cast<unsigned int>(flag));
}