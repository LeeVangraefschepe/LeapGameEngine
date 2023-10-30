#include "BoxCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../SceneGraph/GameObject.h"

#include "../Transform/Transform.h"
#include "Rigidbody.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::BoxCollider::SetupShape()
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(physics::EShape::Box);
	m_pShape->SetSize(m_Size * GetTransform()->GetWorldScale());
}

void leap::BoxCollider::SetSize(const glm::vec3& size)
{
	m_Size = size;
}