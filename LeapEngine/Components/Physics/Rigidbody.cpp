#include "Rigidbody.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../SceneGraph/GameObject.h"
#include "../Transform/Transform.h"
#include "Collider.h"

#include <Data/Rigidbody.h>

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>

void leap::Rigidbody::SetKinematic(bool isKinematic)
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody)  m_pRigidbody = new leap::physics::Rigidbody{};

	m_pRigidbody->SetIsKinematic(isKinematic);
}

void leap::Rigidbody::SetVelocity(const glm::vec3& velocity)
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody) m_pRigidbody = new leap::physics::Rigidbody{};

	m_pRigidbody->SetVelocity(velocity);
}

void leap::Rigidbody::SetVelocity(float x, float y, float z)
{
	SetVelocity(glm::vec3{ x,y,z });
}

void leap::Rigidbody::SetMass(float mass)
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody) m_pRigidbody = new leap::physics::Rigidbody{};

	m_pRigidbody->SetMass(mass);
}

void leap::Rigidbody::Awake()
{
	// Get the physics object for this gameobject
	physics::IPhysicsObject* pObject{ ServiceLocator::GetPhysics().Get(GetGameObject()) };

	// Create a rigidbody
	physics::Rigidbody* pNewRigidbody{ pObject->SetRigidbody(true) };

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

	ApplyShapes(GetGameObject());
}

void leap::Rigidbody::OnDestroy()
{
	// Remove the rigidbody
	ServiceLocator::GetPhysics().Get(GetGameObject())->SetRigidbody(false);
}

void leap::Rigidbody::ApplyShapes(GameObject* pParent) const
{
	const auto pCollidersOnParent{ pParent->GetComponents<Collider>() };

	// Move all the colliders on the gameobject to this rigidbody
	for (auto pCollider : pCollidersOnParent)
	{
		pCollider->Move(this);
	}

	// Move all the colliders on the children to this rigidbody
	for (int i{}; i < pParent->GetChildCount(); ++i)
	{
		GameObject* pChild{ pParent->GetChild(i) };
		if (pChild->GetComponent<Rigidbody>()) continue;

		ApplyShapes(pChild);
	}
}