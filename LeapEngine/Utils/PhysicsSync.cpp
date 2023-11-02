#include "PhysicsSync.h"

#include "../Components/Transform/Transform.h"
#include "../Components/Physics/Collider.h"

#include "../SceneGraph/GameObject.h"

#include <Data/CollisionData.h>

void leap::PhysicsSync::SetTransform(void* pOwner, const glm::vec3& position, const glm::quat& rotation)
{
	Transform* pTransform{ reinterpret_cast<GameObject*>(pOwner)->GetTransform() };

	pTransform->SetWorldPosition(position);
	pTransform->SetWorldRotation(rotation);
}

std::pair<const glm::vec3&, const glm::quat&> leap::PhysicsSync::GetTransform(void* pOwner)
{
	Transform* pTransform{ reinterpret_cast<GameObject*>(pOwner)->GetTransform() };

	return std::make_pair<const glm::vec3&, const glm::quat&>(pTransform->GetWorldPosition(), pTransform->GetWorldRotation());
}

void leap::PhysicsSync::OnCollision(const physics::CollisionData& collision)
{
	Collider* pFirstCollider{ reinterpret_cast<Collider*>(collision.pFirst) };
	Collider* pSecondCollider{ reinterpret_cast<Collider*>(collision.pSecond) };

	pFirstCollider->NotifyCollision(pSecondCollider);
	pSecondCollider->NotifyCollision(pFirstCollider);
}
