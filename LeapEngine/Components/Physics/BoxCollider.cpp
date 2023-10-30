#include "BoxCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::BoxCollider::Awake()
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(physics::EShape::Box);
	m_pShape->SetSize(m_Size * GetTransform()->GetWorldScale());

	// Get the physics object associated with the gameobject
	physics::IPhysicsObject* pObject{ physics.Get(GetGameObject()) };

	// Apply the shape and set the transform of the physics object
	pObject->AddShape(m_pShape.get());
	pObject->SetTransform(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldRotation());
}

void leap::BoxCollider::OnDestroy()
{
	ServiceLocator::GetPhysics().Get(GetGameObject())->RemoveShape(m_pShape.get());
}

void leap::BoxCollider::SetSize(const glm::vec3& size)
{
	m_Size = size;
}
