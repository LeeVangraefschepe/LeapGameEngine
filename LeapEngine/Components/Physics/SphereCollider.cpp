#include "SphereCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;

	if (m_pShape) RescaleShape();
}

void leap::SphereCollider::SetupShape(physics::IPhysicsMaterial* pMaterial)
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape
	m_pShape = physics.CreateShape(this, physics::EShape::Sphere, pMaterial);
	RescaleShape();
}

void leap::SphereCollider::RescaleShape()
{
	// Apply radius
	const auto& scale{ GetTransform()->GetWorldScale() };
	const float transformSize{ std::max(scale.x, std::max(scale.y, scale.z)) };
	m_pShape->SetRadius(m_Radius * transformSize);
}