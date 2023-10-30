#include "SphereCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;
}

void leap::SphereCollider::Awake()
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(physics::EShape::Sphere);

	const auto& scale{ GetTransform()->GetWorldScale() };
	const float transformSize{ std::max(scale.x, std::max(scale.y, scale.z)) };
	m_pShape->SetRadius(m_Radius * transformSize);

	// Get the physics object associated with the gameobject
	physics::IPhysicsObject* pObject{ physics.Get(GetGameObject()) };

	// Apply the shape and set the transform of the physics object
	pObject->AddShape(m_pShape.get());
	pObject->SetTransform(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldRotation());
}

void leap::SphereCollider::OnDestroy()
{
	ServiceLocator::GetPhysics().Get(GetGameObject())->RemoveShape(m_pShape.get());
}
