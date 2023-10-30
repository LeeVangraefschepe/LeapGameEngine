#include "SphereCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::SphereCollider::Awake()
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };
	m_pShape = physics.CreateShape(physics::EShape::Sphere);
	physics::IPhysicsObject* pObject{ physics.Get(GetGameObject()) };
	pObject->AddShape(m_pShape.get());
	pObject->SetTransform(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldRotation());
}

void leap::SphereCollider::OnDestroy()
{
	ServiceLocator::GetPhysics().Get(GetGameObject())->RemoveShape(m_pShape.get());
}
