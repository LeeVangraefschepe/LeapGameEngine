#include "PrintCollision.h"

#include "SceneGraph/GameObject.h"
#include "Components/Physics/Collider.h"

#include <Debug.h>

#include <sstream>

void unag::PrintCollision::Notify(const leap::Collider::CollisionCallback& collision)
{
	std::stringstream ss{};
	ss << "Collision between " << collision.pCollider->GetGameObject()->GetName() << " and " << collision.pOther->GetGameObject()->GetName();
	leap::Debug::Log(ss.str());
}

void unag::PrintCollision::Awake()
{
	GetGameObject()->GetComponent<leap::Collider>()->OnCollision.AddListener(this);
}

void unag::PrintCollision::OnDestroy()
{
	GetGameObject()->GetComponent<leap::Collider>()->OnCollision.RemoveListener(this);
}
