#include "PrintCollision.h"

#include "SceneGraph/GameObject.h"
#include "Components/Physics/Collider.h"

#include <Debug.h>

#include <sstream>

void unag::PrintCollision::Awake()
{
	std::vector<leap::Collider*> pColliders{ GetGameObject()->GetComponents<leap::Collider>() };

	for (leap::Collider* pCollider : pColliders)
	{
		pCollider->OnCollisionEnter.AddListener(&m_CollisionEnter);
		pCollider->OnCollisionStay.AddListener(&m_CollisionStay);
		pCollider->OnCollisionExit.AddListener(&m_CollisionExit);
		pCollider->OnTriggerEnter.AddListener(&m_TriggerEnter);
		pCollider->OnTriggerStay.AddListener(&m_TriggerStay);
		pCollider->OnTriggerExit.AddListener(&m_TriggerExit);
	}
}

void unag::PrintCollision::OnDestroy()
{
	std::vector<leap::Collider*> pColliders{ GetGameObject()->GetComponents<leap::Collider>() };

	for (leap::Collider* pCollider : pColliders)
	{
		pCollider->OnCollisionEnter.RemoveListener(&m_CollisionEnter);
		pCollider->OnCollisionStay.RemoveListener(&m_CollisionStay);
		pCollider->OnCollisionExit.RemoveListener(&m_CollisionExit);
		pCollider->OnTriggerEnter.RemoveListener(&m_TriggerEnter);
		pCollider->OnTriggerStay.RemoveListener(&m_TriggerStay);
		pCollider->OnTriggerExit.RemoveListener(&m_TriggerExit);
	}
}

void unag::PrintCollision::CollisionEnter::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Collision enter between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::CollisionStay::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Collision stay between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::CollisionExit::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Collision exit between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::TriggerEnter::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Trigger enter between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::TriggerStay::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Trigger stay between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::TriggerExit::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Trigger exit between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}
