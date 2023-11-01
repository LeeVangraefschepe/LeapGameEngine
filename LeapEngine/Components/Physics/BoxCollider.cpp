#include "BoxCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../SceneGraph/GameObject.h"

#include "../Transform/Transform.h"
#include "Rigidbody.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::BoxCollider::SetupShape(physics::IPhysicsMaterial* pMaterial)
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(physics::EShape::Box, pMaterial);
	m_pShape->SetSize(m_Size * GetTransform()->GetWorldScale());
}

void leap::BoxCollider::SetSize(const glm::vec3& size)
{
	m_Size = size;
}

void leap::BoxCollider::SetSize(float x, float y, float z)
{
	m_Size.x = x;
	m_Size.y = y;
	m_Size.z = z;
}

void leap::BoxCollider::SetSize(float size)
{
	m_Size.x = size;
	m_Size.y = size;
	m_Size.z = size;
}