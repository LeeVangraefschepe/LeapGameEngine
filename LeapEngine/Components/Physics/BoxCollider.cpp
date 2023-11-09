#include "BoxCollider.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../SceneGraph/GameObject.h"

#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>
#include <Interfaces/IShape.h>

void leap::BoxCollider::SetupShape(physics::IPhysicsMaterial* pMaterial)
{
	physics::IPhysics& physics{ ServiceLocator::GetPhysics() };

	// Create shape and apply the size
	m_pShape = physics.CreateShape(this, physics::EShape::Box, pMaterial);
	RescaleShape();
}

void leap::BoxCollider::RescaleShape()
{
	if (m_pShape) m_pShape->SetSize(m_Size * GetTransform()->GetWorldScale());
}

void leap::BoxCollider::SetSize(const glm::vec3& size)
{
	SetSize(size.x, size.y, size.z);
}

void leap::BoxCollider::SetSize(float x, float y, float z)
{
	m_Size.x = x;
	m_Size.y = y;
	m_Size.z = z;

	if (m_pShape) m_pShape->SetSize(m_Size * GetTransform()->GetWorldScale());
}

void leap::BoxCollider::SetSize(float size)
{
	SetSize(size, size, size);
}