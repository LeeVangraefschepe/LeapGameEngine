#include "PrintCollision.h"

#include "SceneGraph/GameObject.h"
#include "Components/Physics/Collider.h"

#include <Debug.h>

#include <sstream>

#include <ServiceLocator/ServiceLocator.h>
#include <Interfaces/IRenderer.h>

#include <Physics/Physics.h>

void unag::PrintCollision::Start()
{
	leap::RaycastHitInfo hit{};
	if (leap::Physics::Raycast({ 0.0f, 5.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, 10.0f, hit))
	{
		leap::Debug::Log(std::string(hit.pCollider->GetGameObject()->GetName()) + " " + std::to_string(hit.point.y) + " " + std::to_string(hit.normal.y));
	}
}

void unag::PrintCollision::Update()
{
	leap::ServiceLocator::GetRenderer().DrawLine({ 0.0f, 5.0f, 0.0f }, { 0.0f, -5.0f, 0.0f });
	leap::RaycastHitInfo hit{};
	if (leap::Physics::Raycast({ 0.0f, 5.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, 10.0f, hit))
	{
		leap::Debug::Log(std::string(hit.pCollider->GetGameObject()->GetName()) + " " + std::to_string(hit.point.y) + " " + std::to_string(hit.normal.y));
	}
}

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
