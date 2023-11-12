#include "PrintCollision.h"

#include "SceneGraph/GameObject.h"
#include "Components/Physics/Collider.h"

#include <Debug.h>

#include <sstream>

void unag::PrintCollision::OnCollisionEnter(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Collision enter between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::OnCollisionStay(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Collision stay between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::OnCollisionExit(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Collision exit between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::OnTriggerEnter(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Trigger enter between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::OnTriggerStay(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Trigger stay between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::OnTriggerExit(leap::Collider* pCollider, leap::Collider* pOther)
{
	std::stringstream ss{};
	ss << "Trigger exit between " << pCollider->GetGameObject()->GetName() << " and " << pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}
