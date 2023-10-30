#include "Rigidbody.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>

void leap::Rigidbody::SetKinematic(bool isKinematic)
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody)  m_pRigidbody = new leap::physics::IPhysicsObject::Rigidbody{};

	m_pRigidbody->isKinematic = isKinematic;
	m_pRigidbody->isKinematicDirty = true;
}

void leap::Rigidbody::SetVelocity(const glm::vec3& velocity)
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody)  m_pRigidbody = new leap::physics::IPhysicsObject::Rigidbody{};

	m_pRigidbody->velocity = velocity;
	m_pRigidbody->isVelocityDirty = true;
}

void leap::Rigidbody::Awake()
{
	// Get the physics object for this gameobject
	physics::IPhysicsObject* pObject{ ServiceLocator::GetPhysics().Get(GetGameObject()) };

	// Create a rigidbody
	physics::IPhysicsObject::Rigidbody* pNewRigidbody{ pObject->SetRigidbody(true) };

	// If settings were set before awake, apply the settings and destroy the temp rigidbody
	if (m_pRigidbody)
	{
		*pNewRigidbody = *m_pRigidbody;
		delete m_pRigidbody;
	}
	// Cache the rigidbody
	m_pRigidbody = pNewRigidbody;

	// Set the current transform to the physics object
	ServiceLocator::GetPhysics().Get(GetGameObject())->SetTransform(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldRotation());
}

void leap::Rigidbody::OnDestroy()
{
	// Remove the rigidbody
	ServiceLocator::GetPhysics().Get(GetGameObject())->SetRigidbody(false);
}
