#include "CapsuleCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IPhysics.h>

#include "../Transform/Transform.h"

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
	m_pShape = physics.CreateShape(this, physics::EShape::Capsule, pMaterial);
	RescaleShape();
}

void leap::CapsuleCollider::RescaleShape()
{
	const auto& scale{ GetTransform()->GetWorldScale() };

	// Apply radius
	const float transformRadius{ std::max(scale.x, scale.z) };
	m_pShape->SetRadius(m_Radius * transformRadius);

	// Apply height
	m_pShape->SetSize({ 1.0f, m_Height * scale.y, 1.0f });
}
