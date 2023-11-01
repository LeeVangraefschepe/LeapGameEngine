#include "CapsuleCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IPhysics.h>

void leap::CapsuleCollider::SetRadius(float radius)
{
	m_Radius = radius;
}

void leap::CapsuleCollider::SetHeight(float height)
{
	m_Height = height;
}

void leap::CapsuleCollider::SetupShape(physics::IPhysicsMaterial* pMaterial)
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(physics::EShape::Capsule, pMaterial);
	m_pShape->SetRadius(m_Radius);
	m_pShape->SetSize({ 1.0f, m_Height, 1.0f });
}
