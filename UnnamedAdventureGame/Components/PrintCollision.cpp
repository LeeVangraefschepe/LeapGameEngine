#include "PrintCollision.h"

#include "SceneGraph/GameObject.h"
#include "Components/Physics/Collider.h"

#include <Debug.h>

#include <sstream>

void unag::PrintCollision::Awake()
{
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionEnter.AddListener(&m_CollisionEnter);
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionStay.AddListener(&m_CollisionStay);
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionExit.AddListener(&m_CollisionExit);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerEnter.AddListener(&m_TriggerEnter);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerStay.AddListener(&m_TriggerStay);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerExit.AddListener(&m_TriggerExit);
}

void unag::PrintCollision::OnDestroy()
{
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionEnter.RemoveListener(&m_CollisionEnter);
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionStay.RemoveListener(&m_CollisionStay);
	GetGameObject()->GetComponent<leap::Collider>()->OnCollisionExit.RemoveListener(&m_CollisionExit);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerEnter.RemoveListener(&m_TriggerEnter);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerStay.RemoveListener(&m_TriggerStay);
	GetGameObject()->GetComponent<leap::Collider>()->OnTriggerExit.RemoveListener(&m_TriggerExit);
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
