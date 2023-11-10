#include "Rigidbody.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../SceneGraph/GameObject.h"
#include "../Transform/Transform.h"
#include "Collider.h"

#include <Data/Rigidbody.h>

#include <Interfaces/IPhysics.h>
#include <Interfaces/IPhysicsObject.h>

#include <Quaternion.h>

void leap::Rigidbody::SetKinematic(bool isKinematic)
{
	CheckExistence();
	m_pRigidbody->SetIsKinematic(isKinematic);
}

void leap::Rigidbody::SetVelocity(const glm::vec3& velocity)
{
	CheckExistence();
	m_pRigidbody->SetVelocity(velocity);
}

void leap::Rigidbody::SetVelocity(float x, float y, float z)
{
	SetVelocity(glm::vec3{ x,y,z });
}

void leap::Rigidbody::SetAngularVelocity(const glm::vec3& velocity)
{
	CheckExistence();
	m_pRigidbody->SetAngularVelocity(velocity);
}

void leap::Rigidbody::SetAngularVelocity(float x, float y, float z)
{
	SetAngularVelocity(glm::vec3{x, y, z});
}

void leap::Rigidbody::SetMass(float mass)
{
	CheckExistence();
	m_pRigidbody->SetMass(mass);
}

void leap::Rigidbody::SetConstraint(physics::Rigidbody::Constraint::Flag flag, bool enabled)
{
	CheckExistence();
	m_pRigidbody->SetConstraint(flag, enabled);
}

void leap::Rigidbody::Translate(const glm::vec3& displacement)
{
	CheckExistence();
	m_pRigidbody->Translate(displacement);
}

void leap::Rigidbody::Translate(float x, float y, float z) 
{
	Translate({ x,y,z });
}

void leap::Rigidbody::SetPosition(const glm::vec3& position) 
{
	CheckExistence();
	m_pRigidbody->SetPosition(position);
}

void leap::Rigidbody::SetPosition(float x, float y, float z) 
{
	SetPosition({ x, y, z });
}

void leap::Rigidbody::Rotate(const glm::quat& rotationEpsilon)
{
	CheckExistence();
	m_pRigidbody->Rotate(rotationEpsilon);
}

void leap::Rigidbody::Rotate(const glm::vec3& rotationEpsilon, bool degrees)
{
	Rotate(Quaternion::FromEuler(rotationEpsilon, degrees));
}

void leap::Rigidbody::Rotate(float x, float y, float z, bool degrees)
{
	Rotate(Quaternion::FromEuler(x, y, z, degrees));
}

void leap::Rigidbody::SetRotation(const glm::quat& rotation)
{
	CheckExistence();
	m_pRigidbody->SetRotation(rotation);
}

void leap::Rigidbody::SetRotation(const glm::vec3& rotation, bool degrees)
{
	SetRotation(Quaternion::FromEuler(rotation, degrees));
}

void leap::Rigidbody::SetRotation(float x, float y, float z, bool degrees)
{
	SetRotation(Quaternion::FromEuler(x, y, z, degrees));
}

void leap::Rigidbody::AddForce(const glm::vec3& force, physics::ForceMode mode)
{
	CheckExistence();
	m_pRigidbody->AddForce(force, mode);
}

void leap::Rigidbody::AddForce(float x, float y, float z, physics::ForceMode mode)
{
	AddForce({ x,y,z }, mode);
}

void leap::Rigidbody::AddTorque(const glm::vec3& torque, physics::ForceMode mode)
{
	CheckExistence();
	m_pRigidbody->AddTorque(torque, mode);
}

void leap::Rigidbody::AddTorque(float x, float y, float z, physics::ForceMode mode)
{
	AddTorque({ x,y,z }, mode);
}

const glm::vec3& leap::Rigidbody::GetVelocity() const
{
	if (!m_pRigidbody) return m_EmptyVector;

	return m_pRigidbody->GetVelocityFromEngine();
}

const glm::vec3& leap::Rigidbody::GetAngularVelocity() const
{
	if (!m_pRigidbody) return m_EmptyVector;

	return m_pRigidbody->GetAngularVelocityFromEngine();
}

bool leap::Rigidbody::IsConstraint(physics::Rigidbody::Constraint::Flag flag, bool enabled) const
{
	if (!m_pRigidbody) return !enabled;

	const auto& constraints{ m_pRigidbody->GetConstraints() };
	const auto iterator{ std::find_if(begin(constraints), end(constraints), [flag](const auto& constraint) { return constraint.flag == flag; }) };
	
	if (iterator == end(constraints)) return !enabled;

	return iterator->enabled == enabled;
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
		*pNewRigidbody = std::move(*m_pRigidbody);
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

void leap::Rigidbody::CheckExistence()
{
	// If no rigidbody exists (this function is called before awake), create a temporary rigidbody
	if (!m_pRigidbody) m_pRigidbody = new leap::physics::Rigidbody{};
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